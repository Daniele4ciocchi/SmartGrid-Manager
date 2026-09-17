# Architettura e flusso di esecuzione

## 1. Obiettivo computazionale

Il programma valuta strategie di acquisto e vendita di energia su più reti elettriche. Ogni esperimento percorre finestre temporali sovrapposte; in ciascuna finestra una strategia decide le operazioni, il portafoglio contabilizza il denaro, la batteria conserva l'energia e al termine le posizioni vengono liquidate.

Il rendimento della finestra è:

$$
Y = \frac{B_f - B_0}{B_0}
$$

con $B_0$ saldo iniziale e $B_f$ saldo totale finale, cioè saldo disponibile più saldo accantonato.

## 2. Componenti principali

```text
main
  |
  +--> Database ----> utils::readFromDb ----> vector<ElectricityGrid>
  |                                            |
  +--> Simulator ------------------------------+
         |
         +--> StrategyType
         +--> TradingService --> Wallet + Battery + Transaction
         +--> monitor::addYeld
         +--> utils::logExperiment
```

`Source` fornisce il tipo base per una sorgente energetica. `ElectricityGrid` lo specializza mantenendo i prezzi indicizzati per timestamp e le somme prefix dei logaritmi.

## 3. Sequenza operativa

1. `main` definisce i parametri di default e interpreta gli argomenti della riga di comando.
2. `Database` apre `data/valori.db` e `createTables()` prepara lo schema.
3. Con `--initdb`, `utils::insertIntoDb()` importa il file CSV.
4. Con `--simulator`, `Simulator::run()` carica le reti in memoria.
5. Per ogni finestra vengono creati un `Wallet`, una `Battery` e un `TradingService` indipendenti.
6. Per ciascun timestamp la strategia può invocare `buy()` o `sell()`.
7. `sellAll()` liquida le quantità residue usando l'ultimo prezzo valido.
8. Il rendimento viene aggiunto a `monitor::yelds`; media e deviazione standard vengono poi registrate nel CSV degli esperimenti.

## 4. Parallelismo

Il ciclo esterno sulle finestre è parallelizzato con OpenMP. Ogni iterazione possiede lo stato finanziario e quello energetico propri; l'accesso condiviso al vettore dei rendimenti è protetto da una sezione critica:

```cpp
#pragma omp parallel
{
#pragma omp for schedule(static)
    for (int i = GEOMETRIC_WINDOW;
         i <= TIMESTAMP - WINDOW_SIZE; ++i)
    {
        Battery b(BATTERY_CAPACITY);
        Wallet w(WALLET_INITIAL_BALANCE);
        TradingService tradingService;
        // simulazione della finestra [i, i + WINDOW_SIZE)

#pragma omp critical
        monitor::addYeld(
            (w.getTotalBalance() - WALLET_INITIAL_BALANCE)
            / WALLET_INITIAL_BALANCE);
    }
}
```

La rete `reti` è condivisa in lettura dopo il caricamento. Le strutture mutabili di una singola simulazione non sono condivise tra thread.

## 5. Confini e responsabilita

L'architettura separa il modello di stato dalla logica applicativa:

- il dominio non decide quale strategia usare;
- la strategia non modifica direttamente saldo e batteria;
- `TradingService` è il punto che garantisce le condizioni di acquisto e vendita;
- `Database` espone operazioni SQL generiche, mentre `utils` traduce i record in oggetti del dominio.
