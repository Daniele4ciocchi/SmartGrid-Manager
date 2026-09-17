# Documentazione Accademica del Modulo Sorgente Energetico (Source)
**Contesto:** La classe `Source` modella una specifica risorsa energetica all'interno dello Smart Grid Manager. Questo include la definizione di un tipo specifico di generazione o consumo che partecipa al mercato, fungendo da entità chiave per le transazioni e i calcoli di prezzo.

**Architettura e Design:**
Il design è incentrato sulla rappresentazione immutabile dell'identificativo della sorgente (ID) e sull'associazione dinamica dei dati storici di prezzo nel tempo. L'interfaccia minimizza la dipendenza dallo stato globale, rendendola facile da integrare in diversi contesti simulativi.

**API Reference:**
| Metodo | Parametri | Ritorno | Descrizione |
| :--- | :--- | :--- | :--- |
| `Source(int id)` | `id` (`int`) | - | Costruttore che assegna un identificativo unico (ID) alla sorgente. |

**Nota sulle Dipendenze:** L'uso di `Source` dipende dal sistema di identificazione numerico fornito all'inizializzazione. Le classi dipendenti (`ElectricityGrid`, `TradingService`) utilizzano questo ID per la mappatura dei dati temporali.

**Implementazione del Codice Sorgente:**

```cpp
// FILE: Source.h
#ifndef SOURCE_H
#define SOURCE_H

#include <string>
#include "utils.h"

/**
 * @class Source
 * @brief Rappresenta una singola sorgente di energia nel sistema Smart Grid.
 */
class Source 
{
private:
    int id; // Identificativo univoco della fonte
public:
    Source(int id);
};

#endif
```