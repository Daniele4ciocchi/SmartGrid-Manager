# Documentazione Accademica del Progetto SmartGrid-Manager

Questo documento fornisce un'analisi dettagliata e accademica dei moduli software che costituiscono il sistema di gestione della griglia elettrica (SmartGrid-Manager). Data la natura procedurale dell'implementazione, i componenti sono descritti come *moduli* architetturali.

---

## 🏛️ Architettura del Sistema

Il progetto è composto da tre moduli Python principali, ognuno con una responsabilità ben definita nel ciclo di vita dell'esperimento:
1.  **`run_experiments.py`**: Gestione e esecuzione della simulazione.
2.  **`analyze_data.py`**: Analisi statistica dei risultati aggregati.
3.  **`plot_results.py`**: Visualizzazione grafica dei parametri chiave.

---

## 🔬 Moduli Dettagliati

### 1. `run_experiments.py` (Modulo di Esecuzione Simulazione)
Questo modulo è il *driver* principale del sistema, responsabile dell'esecuzione sistematica e riproducibile degli esperimenti parametrici.

**Obiettivo Architetturale:** Automatizzare la generazione e l'esecuzione di tutte le combinazioni possibili di parametri operativa per testare l'efficacia delle strategie di gestione della domanda/offerta energetica.

**Funzionalità Principali:**
*   **Spazio Parametrico (`itertools.product`):** Definisce uno spazio di ricerca completo attraverso la combinazione iterativa di variabili chiave (es.: `BUDGET_VALS`, `GEOM_VALS`, etc.).
*   **Gestione dello Stato (Memoization):** Legge da un file CSV preesistente (`data/esperimenti.csv`) per identificare e saltare automaticamente gli esperimenti già eseguiti, garantendo la riproducibilità e l'efficienza del processo.
*   **Esecuzione Esterna:** Invoca il simulatore core (localizzato in `./build/simulatore/main/simulatore`) passando tutti i parametri di controllo necessari (`--budget`, `--buy`, `--sell`, ecc.).

**Input Necessari:**
1.  `./build/simulatore/main/simulatore`: L'eseguibile binario che rappresenta la simulazione fisica del sistema elettrico.
2.  `data/esperimenti.csv`: File di logging dei risultati precedenti per tracciare lo stato degli esperimenti completati.

---

### 2. `analyze_data.py` (Modulo di Analisi Comparativa)
Questo modulo si occupa dell'analisi statistica comparativa sui parametri numerici estratti dai dati aggregati di più cicli di simulazione.

**Obiettivo Architetturale:** Generare una visione d'insieme delle performance dei diversi set di parametri operativi, riducendo i risultati complessi a metriche comparative essenziali (media e deviazione standard).

**Funzionalità Principali:**
*   **Parsing Dati CSV:** Carica il file `data/esperimenti.csv` utilizzando la libreria Pandas.
*   **Identificazione Variabili:** Seleziona automaticamente i *numerical columns* pertinenti per l'analisi (es.: parametri di guadagno, costi).
*   **Statistiche Descrittive:** Calcola e confronta le statistiche **media (`mean`)** e **deviazione standard (`std`)** dei parametri selezionati attraverso tutti gli esperimenti.
*   **Visualizzazione Risultato:** Genera un diagramma a barre con barre di errore per visualizzare la media attesa (valore centrale) e l'intervallo di incertezza associato (deviazione standard).

---

### 3. `plot_results.py` (Modulo di Visualizzazione Dettagliata)
Questo modulo è dedicato alla generazione di grafici specifici, fornendo una visualizzazione più "accademica" e focalizzata su relazioni causali tra variabili.

**Obiettivo Architetturale:** Isolare e presentare metriche specifiche del sistema elettrico (come l'interazione tra il *Set Aside %* e il *Average Yield (%)*) attraverso grafici statistici avanzati.

**Funzionalità Principali:**
*   **Caricamento Dati Sicuro:** Funzione `load_data` che gestisce l'errore di file mancante e la pulizia dei dati (rimozione header duplicati, conversione forzata in tipo numerico).
*   **Visualizzazione Statistica (Point Plot):** Utilizza `seaborn.pointplot` per analizzare come una variabile indipendente (*Set Aside %*) influenzi una dipendente (*Average Yield %*) in base a un fattore categorico (*Buy Threshold*). Questo è cruciale per identificare soglie operative ottimali.
*   **Gestione Fallback:** Include logica alternativa (`Fallback`) se i dati non supportano l'analisi specifica (es.: mancanza della colonna `Set Aside %`).

**Output Generato:** Immagini PNG ad alta risoluzione, salvate nella directory `data/`.

---
***Nota Metodologica:*** *L'uso di librerie come Pandas e Matplotlib è standard per l'elaborazione scientifica in Python. I parametri esatti del simulatore devono essere passati attraverso le funzioni dedicate, non direttamente nell'esecuzione dello script.*