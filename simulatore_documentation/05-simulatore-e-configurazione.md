# Simulatore e configurazione sperimentale

## 1. Classe `Simulator`

`Simulator` è il coordinatore dell'esecuzione. Riceve per riferimento un `Database` e una `StrategyType`, preservando il database esterno per tutta la durata della simulazione.

```cpp
class Simulator {
public:
    Simulator(Database& db, StrategyType strategy);
    void run();
private:
    Database& db;
    StrategyType strategy;
};
```

Il costruttore è intenzionalmente leggero:

```cpp
Simulator::Simulator(Database &db, StrategyType strategy)
    : db(db), strategy(strategy) {}
```

`run()` carica i dati, crea una simulazione indipendente per ogni finestra temporale, invoca la strategia selezionata e calcola le statistiche aggregate.

## 2. Finestra temporale

Con `WINDOW_SIZE = 300`, una simulazione associata all'indice `i` analizza:

$$
[i, i + WINDOW\_SIZE)
$$

L'indice iniziale varia da `GEOMETRIC_WINDOW` a `TIMESTAMP - WINDOW_SIZE`. L'inizio ritardato permette alle strategie geometriche di disporre di uno storico.

```cpp
for (int i = GEOMETRIC_WINDOW;
     i <= TIMESTAMP - WINDOW_SIZE; ++i)
{
    Battery b(BATTERY_CAPACITY);
    Wallet w(WALLET_INITIAL_BALANCE);
    TradingService tradingService;

    for (int j = i; j < i + WINDOW_SIZE; ++j) {
        switch (strategy) {
        case StrategyType::Random:
            randomChoise(reti, &w, &b, &tradingService, j);
            break;
        case StrategyType::Geometric:
            geometricChoise(reti, &w, &b, &tradingService, j);
            break;
        case StrategyType::SmartGeometric:
            smartgeometricChoise(reti, &w, &b, &tradingService, j);
            break;
        }
    }
    tradingService.sellAll(&reti, &w, &b, i + WINDOW_SIZE - 1);
}
```

## 3. Parametri globali

Le dichiarazioni sono in `definitions.h`, mentre i valori iniziali sono definiti in `main.cpp`:

```cpp
int WINDOW_SIZE = 300;
int GEOMETRIC_WINDOW = 20;
double WALLET_INITIAL_BALANCE = 1000.0;
double SET_ASIDE_PERCENTAGE = 20.0;
double BATTERY_CAPACITY = 1000.0;
double BUDGET = 10.0;
double BUY_THRESHOLD = 0.80;
double SELL_THRESHOLD = 1.00;
int MAX_BUY = 200;
int MAX_SELL = 200;
```

Significato principale:

| Parametro | Funzione |
|---|---|
| `WINDOW_SIZE` | durata della simulazione locale |
| `GEOMETRIC_WINDOW` | ampiezza dello storico per la media geometrica |
| `BUDGET` | valore monetario di ogni operazione |
| `BUY_THRESHOLD` | limite inferiore del rapporto prezzo/media |
| `SELL_THRESHOLD` | limite superiore del rapporto prezzo/media |
| `SET_ASIDE_PERCENTAGE` | percentuale dei ricavi accantonata |
| `BATTERY_CAPACITY` | energia massima immagazzinabile |
| `MAX_BUY`, `MAX_SELL` | limiti della strategia intelligente |

## 4. Interfaccia a riga di comando

Gli argomenti sono interpretati in `main()`:

```text
--strategy random|geometric|smart
--budget NUMERO
--buy NUMERO
--sell NUMERO
--geom INTERO
--window INTERO
--wallet NUMERO
--set-aside NUMERO
--initdb
--simulator
```

`--initdb` importa `data/datafile.csv` nel database; `--simulator` avvia l'esperimento. Le due azioni sono valutate separatamente: l'importazione termina subito dopo il completamento.

## 5. Risultati

Al termine, `Simulator` stampa:

- parametri principali;
- rendimento medio in percentuale;
- deviazione standard;
- intervallo `media - deviazione` / `media + deviazione`.

Inoltre `utils::logExperiment()` aggiunge una riga a `data/esperimenti.csv`. Il rendimento finale di una finestra include il denaro disponibile e quello accantonato, dopo la liquidazione di tutte le posizioni.

## 6. Esempio di esecuzione

```bash
./run.sh --build
./build/simulatore/main/simulatore --initdb
./build/simulatore/main/simulatore --simulator \
    --strategy smart --geom 20 --window 300 --budget 10
```

Il percorso dell'eseguibile può dipendere dalla configurazione CMake usata per la compilazione; lo script `run.sh` resta il punto di ingresso raccomandato dal progetto.
