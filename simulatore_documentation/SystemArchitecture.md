# Documentazione Architetturale del Sistema Smart Grid Manager
**Contesto:** Questo documento fornisce una panoramica ad alto livello dell'architettura software del sistema, descrivendo il flusso di esecuzione e le interdipendenze tra i moduli principali. È fondamentale per comprendere come il simulatore orchestra l'interazione tra transazioni economiche, fisica e analisi statistica.

**Flusso Operativo (Execution Flow):**
1.  **Setup (InitDB):** L'applicazione inizia verificando la necessità di popolare il database (`data/valori.db`). Se necessario, `utils::insertIntoDb` carica i dati storici di prezzo da un file CSV esterno (`DB_PATH`), stabilendo le serie temporali per ogni sorgente energetica.
2.  **Setup Simulation:** Viene istanziato l'oggetto `Database` e vengono create tutte le tabelle necessarie (es. esperimento, yeld).
3.  **Configurazione:** Il modulo principale (`main`) analizza gli argomenti della riga di comando per caricare i parametri operativi (Budget, Soglie di Acquisto/Vendita, Capacità Batteria) e determinare la `StrategyType` da utilizzare.
4.  **Esecuzione Simulatore:** L'oggetto `Simulator` viene istanziato con le dipendenze (`Database`, `Strategy`). Il metodo `run()` avvia il ciclo simulativo parallelo (tramite OpenMP), analizzando finestre temporali sovrapposte.
5.  **Ciclo Temporale (Window):** Per ogni finestra, vengono eseguite iterazioni di transazione basate sulla strategia scelta (`randomChoise`, `geometricChoise`, ecc.). Ogni transazione chiama i servizi di `TradingService` che a loro volta modificano lo stato del `Wallet` e della `Battery`.
6.  **Liquidazione Finale:** Al termine della finestra, avviene la liquidazione finale delle posizioni energetiche residue, aggiornando il bilancio totale.
7.  **Analisi Statistica:** Alla fine dell'intero run, i risultati (rendimento medio e deviazione standard) vengono calcolati da `monitor` e salvati nel database (`yeld`), completando l'esperimento.

**Componenti Chiave e Interdipendenze:**
*   **Database:** Funziona come il repository persistente di tutti i dati storici (prezzi, risultati sperimentali).
*   **Simulator:** Il coordinatore principale che gestisce il loop temporale e le risorse hardware/economiche.
*   **TradingService:** Agisce da gatekeeper tra tutte le operazioni transazionali.

**Parametri Esecutivi Importanti (`main`):**
I parametri di configurazione possono essere passati via linea di comando per la calibrazione:
| Argomento | Tipo | Descrizione | Intervallo tipico |
| :--- | :--- | :--- | :--- |
| `--strategy` | Stringa | Determina l'algoritmo decisionale (random, geometric, smart). | `random`, `geometric`, `smart` |
| `--budget` | Double | Budget iniziale disponibile per le transazioni. | $10.0 - 1000.0$ |
| `--buy` / `--sell` | Double | Soglie di prezzo relative al calcolo (es. rapporto prezzo/media). | $0.5 - 2.0$ |
| `--geom` | Integer | Finestra temporale per il calcolo della media geometrica ($GEOMETRIC\_WINDOW$). | $10 - 50$ |

**Implementazione del Codice Sorgente:**
```cpp
// FILE: main.cpp
#include "utils.h"
#include "definitions.h"
#include "Database.h"
#include "Simulator.h"
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <stdexcept>

// ... (Variabili globali di tuning: WINDOW_SIZE, GEOMETRIC_WINDOW, ecc.)

static StrategyType parseStrategy(const std::string &value) { /* ... */ }

int main(int argc, char **argv) {
    // 1. Parsing argomenti per il tuning dei test (Budget, Soglie, etc.)
    // 2. Setup Database: db("data/valori.db"); db.createTables();
    
    // Controllo se è richiesto l'importazione dei dati storici
    bool runInit = false;
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "--initdb") runInit = true;
    }

    // Se è necessario, carica i dati di esempio dal DB_PATH.
    if (runInit) { /* ... */ }
    
    // 3. Esecuzione Simulatore: Analizza gli argomenti per trovare --simulator e la strategia.
    // Inizializza l'oggetto Simulator(db, strategy).
    // Chiama simulator.run();
}
```