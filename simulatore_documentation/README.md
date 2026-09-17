# Documentazione accademica del simulatore Smart Grid

## Scopo

Questa raccolta descrive il simulatore di compravendita di energia elettrica implementato in C++. L'analisi segue il codice sorgente e distingue il modello del dominio, i servizi applicativi, la persistenza, le strategie decisionali e il protocollo sperimentale.

## Indice

1. [Architettura e flusso](01-architettura-e-flusso.md)
2. [Modello del dominio](02-modello-del-dominio.md)
3. [Persistenza e servizio di trading](03-persistenza-e-trading.md)
4. [Strategie, utilita e monitoraggio](04-strategie-e-statistiche.md)
5. [Simulatore e configurazione sperimentale](05-simulatore-e-configurazione.md)

## Organizzazione del codice

- `simulatore/Classi/`: `Source`, `ElectricityGrid`, `Battery`, `Wallet` e la struttura `Transaction`.
- `simulatore/services/`: `TradingService`.
- `simulatore/db/`: `Database`.
- `simulatore/utils/`: caricamento dati, strategie e monitoraggio dei rendimenti.
- `simulatore/simulator/`: coordinamento dell'esperimento tramite `Simulator`.
- `simulatore/main/`: inizializzazione dei parametri e punto di ingresso.

## Nota metodologica

Gli estratti di codice sono riportati per chiarire il comportamento dell'implementazione. I nomi originali, inclusi `Avaiable`, `Set_Aside`, `yeld` e `Choise`, sono conservati per corrispondere al sorgente.
