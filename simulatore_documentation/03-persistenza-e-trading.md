# Persistenza e servizio di trading

## 1. `Database`

`Database` incapsula la connessione SQLite e offre una piccola interfaccia per apertura, chiusura, esecuzione di comandi e interrogazioni.

```cpp
class Database {
private:
    sqlite3* db;
public:
    Database(const std::string& filename);
    ~Database();
    bool open();
    void close();
    bool execute(const std::string& sql);
    std::vector<std::vector<std::string>> select(const std::string& sql);
    bool createTables();
};
```

Il costruttore invoca `sqlite3_open`; il distruttore chiama `close()`, rendendo RAII la gestione della connessione. `select()` converte ogni riga SQLite in `vector<string>` e rappresenta i valori SQL nulli con la stringa `"NULL"`.

### Schema relazionale

```sql
CREATE TABLE IF NOT EXISTS sorgente (
    id INTEGER PRIMARY KEY AUTOINCREMENT
);

CREATE TABLE IF NOT EXISTS valore (
    ts INTEGER NOT NULL,
    sorgente INTEGER NOT NULL,
    prezzo REAL NOT NULL,
    prezzo_log REAL NOT NULL,
    PRIMARY KEY (ts, sorgente),
    FOREIGN KEY (sorgente) REFERENCES sorgente(id)
);
```

`prezzo_log` viene persistito per evitare di ricalcolare il log durante il caricamento. L'importazione usa una transazione e `INSERT OR IGNORE`, perciò le righe già presenti non vengono duplicate.

## 2. `TradingService`

`TradingService` coordina le operazioni economiche ed energetiche. Mantiene lo storico delle transazioni e la quantità ancora posseduta per sorgente:

```cpp
class TradingService {
private:
    std::vector<Transaction> transactions{};
    std::map<Source *, double> sourceQuantity;
public:
    bool canBuy(Source*, Wallet*, Battery*, double balance, double price);
    bool canSell(Source*, Wallet*, Battery*, double balance, double price);
    void buy(Source*, Wallet*, Battery*, double balance, double price);
    void sell(Source*, Wallet*, Battery*, double balance, double price);
    void sellAll(std::vector<ElectricityGrid>*, Wallet*, Battery*, int ts);
};
```

### Acquisto

Dato un valore monetario $M$ e un prezzo unitario $p$, la quantità è:

$$q = \frac{M}{p}$$

L'acquisto è ammissibile se il wallet dispone di almeno $M$ e la batteria ha almeno $q$ kWh liberi:

```cpp
bool TradingService::canBuy(Source*, Wallet *w, Battery *b,
                            double balance, double price)
{
    double quantity = balance / price;
    return w->getAvaiable() >= balance
        && b->getFreeSpace() >= quantity;
}
```

`buy()` verifica la precondizione, preleva il denaro, carica la batteria, aggiunge una `Transaction` e incrementa `sourceQuantity[source]`.

### Vendita

La vendita richiede contemporaneamente energia sufficiente nella batteria e quantità sufficiente associata alla sorgente:

```cpp
bool TradingService::canSell(Source *source, Wallet*, Battery *b,
                             double balance, double price)
{
    double quantity = balance / price;
    return b->getLevel() >= quantity
        && sourceQuantity[source] >= quantity;
}
```

Dopo la vendita, `Wallet::deposit()` accredita l'importo, `Battery::discharge()` rimuove l'energia e la quantità della sorgente viene decrementata.

### Liquidazione finale

`sellAll()` percorre tutte le reti, individua l'ultimo timestamp con prezzo positivo e vende la posizione residua a quel prezzo. In questo modo il rendimento della finestra include anche l'energia non venduta durante le decisioni intermedie.

## 3. Invarianti del servizio

1. Nessun acquisto può portare il saldo disponibile sotto zero.
2. Nessun acquisto può superare la capacità della batteria.
3. Nessuna vendita può scaricare la batteria oltre il livello corrente.
4. Una vendita non può superare la quantità acquistata per la stessa sorgente.
5. Ogni operazione accettata produce una registrazione nello storico.

Le eccezioni generate da `Wallet` o `Battery` vengono catturate da `buy()` e `sell()`; una transazione incompleta non viene aggiunta allo storico.
