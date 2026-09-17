# Modello del dominio

## 1. `Source`

`Source` è la classe base minimale per una fonte energetica. Non possiede attributi, ma fornisce un tipo comune che consente a `TradingService` di indicizzare le quantità acquistate tramite `Source*`.

```cpp
class Source {
public:
    Source();
    ~Source();
};
```

La distruzione non è polimorfica (`~Source()` non è `virtual`), ma nel programma i puntatori a `Source` riferiscono oggetti `ElectricityGrid` senza essere distrutti attraverso il tipo base.

## 2. `ElectricityGrid`

Rappresenta una rete elettrica e la sua serie temporale di prezzi. La classe eredita da `Source` e mantiene un identificativo persistente, i prezzi originali e due strutture prefix per calcolare rapidamente la media geometrica.

```cpp
class ElectricityGrid : public Source {
private:
    std::vector<double> prices;
    std::vector<double> prices_log;
    std::vector<int> valid_prices_count;
    int sourceId = -1;
};
```

### Operazioni fondamentali

```cpp
void addPrice(double prezzo) { prices.push_back(prezzo); }

double getPriceByTs(int ts) const {
    if (ts < 0 || ts >= static_cast<int>(prices.size()))
        return -1.0;
    return prices[ts];
}
```

`getPriceByTs()` usa `-1.0` come valore sentinella per un timestamp assente. `addPriceLog()` aggiorna una somma cumulativa dei logaritmi e il numero cumulativo dei prezzi positivi:

```cpp
void ElectricityGrid::addPriceLog(double prezzo_log)
{
    double last = prices_log.empty() ? 0.0 : prices_log.back();
    prices_log.push_back(last + prezzo_log);

    int count = valid_prices_count.empty()
              ? 0 : valid_prices_count.back();
    int valid = (!prices.empty() && prices.back() > 0.0) ? 1 : 0;
    valid_prices_count.push_back(count + valid);
}
```

Per un intervallo semiaperto $[start, end)$, la somma viene ottenuta in $O(1)$ come differenza tra prefix sum:

```cpp
double getPriceLogSum(int start, int end) const {
    if (start >= end) return 0.0;
    return start == 0
        ? prices_log[end - 1]
        : prices_log[end - 1] - prices_log[start - 1];
}
```

Ne consegue che il calcolo della media geometrica non deve ripercorrere tutti i prezzi della finestra.

## 3. `Battery`

`Battery` modella l'energia acquistata e disponibile per la vendita. `capacity` è la capacità massima in kWh, mentre `level` è il contenuto corrente.

```cpp
class Battery {
private:
    double capacity;
    double level;
public:
    Battery(double capacity) : capacity(capacity), level(0) {}
    double getFreeSpace() const { return capacity - level; }
    double charge(double charge);
    double discharge(double discharge);
};
```

Le transizioni valide sono vincolate da due invarianti:

- $0 \le level \le capacity$;
- una carica non può superare `capacity - level`;
- una scarica non può superare `level`.

```cpp
double Battery::charge(double amount)
{
    if (amount > capacity - level)
        throw std::out_of_range("La carica supera la capacità residua");
    level += amount;
    return amount;
}
```

`discharge()` applica il vincolo duale e diminuisce il livello.

## 4. `Wallet`

`Wallet` separa il denaro immediatamente spendibile (`Avaiable`) da quello accantonato (`Set_Aside`). Il saldo totale è la loro somma.

```cpp
class Wallet {
private:
    double Avaiable;
    double Set_Aside;
public:
    Wallet(double initial_balance)
        : Avaiable(initial_balance), Set_Aside(0) {}
    double getTotalBalance() {
        return Avaiable + Set_Aside;
    }
    double deposit(double amount);
    double withdraw(double amount);
};
```

Il deposito applica la percentuale globale di accantonamento:

```cpp
double Wallet::deposit(double amount)
{
    Set_Aside += amount * SET_ASIDE_PERCENTAGE / 100.0;
    Avaiable += amount * (100.0 - SET_ASIDE_PERCENTAGE) / 100.0;
    return Avaiable;
}
```

Il prelievo è consentito solo se il saldo disponibile è sufficiente; in caso contrario viene lanciata `std::out_of_range`. Il saldo accantonato non finanzia gli acquisti, ma contribuisce alla valutazione finale.

## 5. `Transaction`

`Transaction` è una struttura di registrazione, non una classe con comportamento. Descrive una singola operazione:

```cpp
struct Transaction {
    Source *source;
    double quantity;
    double price;
    double balance;
    bool in_out; // true: vendita, false: acquisto
};
```

`quantity` è espressa in kWh, `price` è il prezzo unitario e `balance` è il valore monetario dell'operazione. Il puntatore `source` identifica la rete coinvolta.
