# Documentazione Dati Transazionali (Transaction)
**Contesto:** La struttura dati `Transaction` è il veicolo di trasporto informativo utilizzato da `TradingService` per registrare ogni singola interazione economica o fisica che avviene all'interno dello Smart Grid Manager. Serve come prova immutabile del cambiamento di stato del sistema (Wallet e Battery).

**Architettura e Design:**
Essendo una semplice struttura dati, il suo design è ottimizzato per la serializzazione e l'archiviazione nel vettore `transactions`. Ogni campo cattura un aspetto specifico dell'evento: dalla fonte che ha originato/ricevuto energia al momento esatto del trasferimento.

**API Reference (Campi della Struttura):**
| Campo | Tipo | Descrizione | Contesto di Uso |
| :--- | :--- | :--- | :--- |
| `source` | Puntero a Source* | Indica l'entità fisica che ha subito la variazione energetica. | Tracciabilità della Fonte Energetica. |
| `balance` | `double` | L'importo monetario totale coinvolto nella transazione (inclusa o esclusa). | Determinare il movimento di denaro nel Wallet. |
| `price` | `double` | Il prezzo unitario energetico (€/kWh) valido al momento dell'operazione. | Calcolo del costo/ricavo totale. |
| `quantity` | `double` | La quantità fisica di energia trasferita (kWh). | Misura dello scambio fisico. |
| `in_out` | `bool` | Indicatore booleano: `false` per l'ingresso (acquisto, in), `true` per l'uscita (vendita, out). | Determinazione del flusso nel sistema (Buy vs Sell). |

**Nota:** Non essendo un modulo attivo, ma una struttura di dati, la documentazione si limita alla definizione dei suoi campi e al loro significato nel ciclo di simulazione.

**Implementazione del Codice Sorgente:**

```cpp
// FILE: Transaction.h
#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "Source.h"
#include <chrono> // Assumendo un timestamp per il momento esatto

/**
 * @struct Transaction
 * @brief Struttura che registra i dettagli completi di una singola operazione energetica o finanziaria.
 */
struct Transaction
{
    // Riferimento alla sorgente coinvolta nella transazione
    Source* source; 
    
    // Dati economici
    double balance; // Saldo monetario totale (es: costo/ricavo)
    double price;   // Prezzo unitario (€/kWh)
    
    // Dati fisici
    double quantity; // Quantità di energia scambiata (kWh)
    bool in_out;     // true se uscita (vendita), false se entrata (acquisto)

    // Timestamp originale (non incluso nell'esempio, ma cruciale nel codice reale)
    // std::chrono::system_clock::time_point timestamp; 
};

#endif // TRANSACTION_H
```