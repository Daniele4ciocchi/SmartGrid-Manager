# Guida al simulatore SmartGrid Manager

Questo documento descrive in modo operativo cosa fa il simulatore, quali dati usa, come prende le decisioni e quali risultati produce.

Ho preparato la guida leggendo il codice sorgente e i risultati già presenti nel repository, senza eseguire il programma.

## Scopo del simulatore

Il simulatore prova a massimizzare il rendimento di un portafoglio virtuale che compra e vende energia su più reti elettriche.

L'idea generale è questa:

1. legge una serie storica di prezzi dall'archivio locale;
2. costruisce per ogni rete una sequenza temporale di prezzi;
3. scorre finestre temporali di simulazione;
4. decide quando comprare o vendere usando una strategia basata sulla media geometrica;
5. chiude tutte le posizioni alla fine di ogni finestra;
6. salva statistiche aggregate del rendimento.

## Flusso dei dati

Il flusso principale è questo:

1. `data/datafile.csv` contiene i prezzi grezzi.
2. `utils::insertIntoDb(...)` importa quei valori nel database SQLite `data/valori.db`.
3. `utils::readFromDb(...)` rilegge i dati dal database e li ricostruisce in una lista di `ElectricityGrid`.
4. `Simulator::run()` esegue la simulazione sulle reti caricate.

Il punto di ingresso dell'applicazione è [simulatore/main/src/main.cpp](simulatore/main/src/main.cpp).

## Parametri principali

Le costanti di default sono definite in [simulatore/main/src/main.cpp](simulatore/main/src/main.cpp) e in [simulatore/define/include/definitions.h](simulatore/define/include/definitions.h).

Valori principali:

- `WINDOW_SIZE = 300`: lunghezza della finestra di simulazione.
- `GEOMETRIC_WINDOW = 20`: finestra usata per calcolare la media geometrica.
- `WALLET_INITIAL_BALANCE = 1000.0`: capitale iniziale del portafoglio.
- `SET_ASIDE_PERCENTAGE = 20.0`: percentuale del ricavato che viene accantonata.
- `BATTERY_CAPACITY = 1000.0`: capacità massima della batteria.
- `BUDGET = 10.0`: importo usato per ogni ordine di acquisto/vendita.
- `BUY_THRESHOLD = 0.80`: soglia per considerare un prezzo conveniente in acquisto.
- `SELL_THRESHOLD = 1.00`: soglia per considerare un prezzo alto in vendita.
- `MAX_BUY = 200` e `MAX_SELL = 200`: limiti massimi di operazioni per timestamp.

I parametri possono essere passati da riga di comando:

- `--budget <valore>`
- `--buy <valore>`
- `--sell <valore>`
- `--geom <valore>`
- `--window <valore>`
- `--wallet <valore>`
- `--set-aside <valore>`
- `--strategy <nome>` (`random`, `geometric` oppure `smart`; predefinita: `geometric`)

Per avviare l'importazione dati o la simulazione sono previsti anche:

- `--initdb`
- `--simulator`

## Cosa fa il simulatore, passo per passo

La logica centrale vive in [simulatore/simulator/src/Simulator.cpp](simulatore/simulator/src/Simulator.cpp).

Per ogni finestra temporale:

1. crea un nuovo `Battery` e un nuovo `Wallet` con i valori iniziali;
2. crea un `TradingService` per gestire operazioni e posizioni;
3. percorre i timestamp della finestra;
4. a ogni timestamp applica la strategia `smartgeometricChoise`;
5. alla fine della finestra vende tutto quello che è rimasto aperto;
6. calcola il rendimento della finestra;
7. lo aggiunge al monitor globale;
8. alla fine stampa media, deviazione standard e range del rendimento;
9. salva i dati aggregati in CSV con `utils::logExperiment(...)`.

## Strategia usata davvero

Nel codice attuale la strategia attiva è `smartgeometricChoise`, definita in [simulatore/utils/src/strategy.cpp](simulatore/utils/src/strategy.cpp).

Le altre due strategie esistono ma nel flusso principale sono commentate:

- `randomChoise`
- `geometricChoise`

### Come decide `smartgeometricChoise`

Per ogni timestamp:

1. legge il prezzo corrente di ogni rete elettrica;
2. calcola la media geometrica degli ultimi `GEOMETRIC_WINDOW` prezzi validi;
3. calcola il rapporto `prezzo / media_geometrica`;
4. ordina tutte le reti in base a questo rapporto;
5. compra prima dalle reti con rapporto più basso, finché il rapporto resta sotto `BUY_THRESHOLD`;
6. vende prima sulle reti con rapporto più alto, finché il rapporto resta sopra `SELL_THRESHOLD`;
7. non supera i limiti `MAX_BUY` e `MAX_SELL`.

Ogni ordine usa il budget fisso `BUDGET`.

## Come vengono verificati gli acquisti e le vendite

La parte di esecuzione concreta è in [simulatore/services/src/TradingService.cpp](simulatore/services/src/TradingService.cpp).

### Acquisto

Un acquisto è possibile solo se:

- il portafoglio ha disponibilità sufficiente;
- la batteria ha spazio residuo sufficiente;
- la quantità convertita dal budget al prezzo corrente entra nella batteria.

Quando l'acquisto avviene:

1. il portafoglio viene addebitato;
2. la batteria viene caricata;
3. la posizione per quella rete viene registrata;
4. la quantità acquistata viene accumulata nella mappa `sourceQuantity`.

### Vendita

Una vendita è possibile solo se:

- la batteria contiene abbastanza energia;
- esiste abbastanza quantità associata a quella rete;
- la quantità venduta corrisponde al budget convertito al prezzo corrente.

Quando la vendita avviene:

1. il portafoglio riceve il ricavo;
2. la batteria viene scaricata;
3. la posizione della rete viene ridotta;
4. la quantità residua viene aggiornata.

### Chiusura finale delle posizioni

Alla fine di ogni finestra, `sellAll(...)` prova a vendere tutto ciò che è ancora aperto usando l'ultimo prezzo disponibile non nullo della rete.

Questo serve a chiudere il ciclo di simulazione senza lasciare posizioni aperte tra una finestra e la successiva.

## Portafoglio e batteria

Le classi principali sono definite in:

- [simulatore/Classi/include/Wallet.h](simulatore/Classi/include/Wallet.h)
- [simulatore/Classi/include/Battery.h](simulatore/Classi/include/Battery.h)

### Wallet

Il wallet mantiene due valori:

- saldo disponibile;
- saldo accantonato.

Quando incassa una vendita, il ricavo viene diviso in due parti:

- una percentuale resta accantonata;
- il resto torna disponibile per nuovi acquisti.

Il rendimento finale viene misurato sul totale del wallet, cioè saldo disponibile più accantonato.

### Batteria

La batteria rappresenta l'energia fisica detenuta dal simulatore.

- `charge(...)` aumenta il livello solo se c'è capacità residua;
- `discharge(...)` diminuisce il livello solo se l'energia disponibile basta.

Se una finestra porta a un'eccezione di overflow o underflow, quella finestra viene saltata nel ciclo di simulazione.

## Media geometrica

La media geometrica è calcolata nei helper di [simulatore/utils/src/utils.cpp](simulatore/utils/src/utils.cpp).

Il meccanismo è questo:

1. ogni prezzo viene salvato insieme al suo logaritmo;
2. i logaritmi vengono accumulati per rete;
3. la media geometrica si ottiene esponenziando la media dei logaritmi sui prezzi validi della finestra.

I prezzi non validi vengono trattati come assenti nel conteggio.

## Statistiche finali

Il modulo [simulatore/utils/src/Monitor.cpp](simulatore/utils/src/Monitor.cpp) raccoglie tutti i rendimenti delle finestre.

Alla fine della simulazione vengono stampati:

- budget usato;
- soglia buy;
- soglia sell;
- percentuale accantonata;
- finestra geometrica;
- media del rendimento;
- deviazione standard del rendimento;
- intervallo `media ± deviazione standard`.

Poi `utils::logExperiment(...)` aggiunge una riga a `data/esperimenti.csv`.

## Risultati già presenti

Nel file [data/esperimenti.csv](data/esperimenti.csv) sono già presenti alcuni risultati della strategia `smartGeometricChoise`.

I valori mostrano, per esempio:

- rendimento medio positivo intorno al 6.9% - 9.6% in alcune configurazioni;
- guadagno finale medio nell'ordine di circa 69 - 96 su un capitale iniziale di 1000.

Il file [data/risultati.txt](data/risultati.txt) risulta invece vuoto al momento.

## Come si avvia il progetto

Lo script [run.sh](run.sh) lancia l'eseguibile principale nella build e accetta `--build` per compilare prima.

Uso tipico:

```bash
./run.sh --build --simulator
```

Se vuoi importare i dati nel database invece di simulare:

```bash
./run.sh --build --initdb
```

Lo script [run_tests.sh](run_tests.sh) non contiene test automatici unitari: è un runner di esperimenti che compila il progetto e rilancia il simulatore con diverse combinazioni di parametri.

## Valutazione tecnica

Dal codice, il simulatore è coerente nel suo flusso principale:

- importa i dati;
- costruisce le serie temporali;
- applica una strategia deterministica su media geometrica;
- chiude le posizioni;
- misura il rendimento;
- salva i risultati.

Ci sono però due note importanti da tenere presenti:

1. la verifica qui è statica, non eseguita, quindi non posso certificare il comportamento runtime reale;
2. la qualità dei risultati dipende molto dai parametri scelti e dalla completezza dei dati nel database.

## File utili da leggere

- [simulatore/main/src/main.cpp](simulatore/main/src/main.cpp)
- [simulatore/simulator/src/Simulator.cpp](simulatore/simulator/src/Simulator.cpp)
- [simulatore/utils/src/strategy.cpp](simulatore/utils/src/strategy.cpp)
- [simulatore/services/src/TradingService.cpp](simulatore/services/src/TradingService.cpp)
- [simulatore/utils/src/utils.cpp](simulatore/utils/src/utils.cpp)
- [data/esperimenti.csv](data/esperimenti.csv)
