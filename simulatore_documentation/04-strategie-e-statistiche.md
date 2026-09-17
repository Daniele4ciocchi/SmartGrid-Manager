# Strategie, utilita e statistiche

## 1. `StrategyType`

L'enumerazione identifica il comportamento decisionale:

```cpp
enum class StrategyType {
    Random,
    Geometric,
    SmartGeometric
};
```

`strategyName()` converte il valore in una stringa usata nel CSV degli esperimenti.

## 2. Strategia casuale

`randomChoise()` seleziona, per ogni rete e timestamp, una delle azioni `niente`, `buy` o `sell` con probabilità uniforme. Il budget per operazione è `BUDGET`.

```cpp
std::uniform_int_distribution<int> action(0, 2);
int choice = action(gen);

if (choice == 1 && ts->canBuy(&r, w, b, budget, price))
    ts->buy(&r, w, b, budget, price);
else if (choice == 2 && ts->canSell(&r, w, b, budget, price))
    ts->sell(&r, w, b, budget, price);
```

Il generatore `thread_local` evita che thread diversi condividano lo stesso stato casuale.

## 3. Strategia geometrica

`geometricChoise()` confronta il prezzo corrente con la media geometrica della finestra storica:

$$
G = \exp\left(\frac{1}{n}\sum_{k=1}^{n}\log p_k\right)
$$

La decisione usa il rapporto $r = p/G$:

- se $r < BUY\_THRESHOLD$, compra;
- se $r > SELL\_THRESHOLD$, vende;
- altrimenti non opera.

```cpp
double geoMean = utils::geometricMean(r, j);
if (price / geoMean < BUY_THRESHOLD)
    ts->buy(&r, w, b, BUDGET, price);
else if (price / geoMean > SELL_THRESHOLD)
    ts->sell(&r, w, b, BUDGET, price);
```

La strategia resta inattiva prima che siano disponibili almeno `GEOMETRIC_WINDOW` timestamp.

## 4. Strategia SmartGeometric

`smartgeometricChoise()` raccoglie i rapporti prezzo/media di tutte le reti in un `std::set`, quindi:

1. ordina le reti dal rapporto più basso al più alto;
2. acquista al massimo `MAX_BUY` reti sotto la soglia di acquisto;
3. percorre l'ordinamento inverso;
4. vende al massimo `MAX_SELL` reti sopra la soglia di vendita.

Questa strategia introduce una selezione competitiva tra reti, mentre la strategia geometrica valuta ogni rete indipendentemente.

## 5. Namespace `utils`

### Caricamento

`readFromDb()` esegue:

```sql
SELECT sorgente, ts, prezzo, prezzo_log
FROM valore
ORDER BY sorgente, ts;
```

Per ogni riga crea, quando necessario, l'oggetto `ElectricityGrid` corrispondente e aggiunge prezzo e logaritmo.

### Media geometrica

```cpp
double geometricMean(const ElectricityGrid& grid, int ts)
{
    int start = std::max(0, ts - GEOMETRIC_WINDOW);
    int end = std::min(ts, grid.getPricesSize());
    int count = grid.getValidCount(start, end);
    if (start >= end || count == 0) return -1.0;
    return exp(grid.getPriceLogSum(start, end) / count);
}
```

Il valore `-1.0` segnala dati insufficienti. I prezzi non positivi sono esclusi dal conteggio valido.

### Registrazione dell'esperimento

`logExperiment()` crea l'intestazione del CSV al primo utilizzo e registra parametri, rendimento medio, deviazione standard e guadagno finale stimato.

## 6. Namespace `monitor`

Il namespace mantiene il vettore globale `yelds` e fornisce operazioni statistiche:

```cpp
double calculateAverage()
{
    double sum = 0.0;
    for (double yeld : yelds) sum += yeld;
    return yelds.empty() ? 0.0 : sum / yelds.size();
}
```

La deviazione standard implementata è campionaria:

$$
 s = \sqrt{\frac{1}{n-1}\sum_{i=1}^{n}(Y_i-\bar{Y})^2}
$$

Per meno di due osservazioni restituisce `0.0`.
