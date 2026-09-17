# Documentazione Accademica del Modulo Rete Elettrica (ElectricityGrid)
**Contesto:** La classe `ElectricityGrid` è il contenitore fondamentale che aggrega i dati storici di prezzo per una specifica sorgente energetica su un intervallo di tempo definito (Timestamp, $t$). È l'interfaccia principale con cui i componenti simulativi interagiscono per determinare il costo dell'energia.

**Architettura e Design:**
Questo modulo utilizza strutture dati ottimizzate per la gestione sequenziale dei prezzi nel tempo (Series Time-based Data Structure). Offre metodi di accesso specifici, come `getPriceByTs(t)`, che permettono un recupero efficiente del dato richiesto senza dover iterare su tutti i punti temporali.

**API Reference:**
| Metodo | Parametri | Ritorno | Descrizione |
| :--- | :--- | :--- | :--- |
| `addPrice(double price)` | `price` (`double`) | - | Aggiunge un prezzo all'intervallo temporale in tempo crescente. |
| `addPriceLog(double log_price)` | `log_price` (`double`) | - | Aggiunge il valore logaritmico del prezzo (utilizzato per calcolo geometrico). |
| `getPriceByTs(int ts)` | `ts` (`int`) | `double` | Recupera il prezzo associato al timestamp $t$. Ritorna $-1$ se non disponibile. |
| `getPricesSize()` | Nessuno | `size_t` | Restituisce la dimensione totale della serie temporale di prezzi registrati. |

**Implementazione del Codice Sorgente:**

```cpp
// FILE: ElectricityGrid.h
#ifndef ELECTRICITYGRID_H
#define ELECTRICITYGRID_H

#include "Source.h"
#include <vector>
#include <map>
#include <algorithm>

/**
 * @class ElectricityGrid
 * @brief Contenitore per la serie temporale di prezzi energetici di una specifica sorgente.
 */
class ElectricityGrid 
{
private:
    Source* source; // Riferimento alla sorgente fisica
    std::vector<double> prices;      // Prezzi grezzi (kWh)
    std::vector<double> priceLogs;   // Logaritmi naturali dei prezzi (per calcolo geometrico)

public:
    ElectricityGrid(Source* source);
    ~ElectricityGrid();

    void addPrice(double price);
    void addPriceLog(double log_price);
    
    double getPriceByTs(int ts) const;
    size_t getPricesSize() const { return prices.size(); }

private:
    // Funzione di utilità per calcolare la media e altre statistiche...
};

#endif // ELECTRICITYGRID_H
```