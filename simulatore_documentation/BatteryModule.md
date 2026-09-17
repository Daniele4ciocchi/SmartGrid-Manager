# Documentazione Accademica del Modulo Accumulo Energetico (Battery)
**Contesto:** Il modulo `Battery` simula l'accumulo e lo scaricamento di energia elettrica in un sistema Smart Grid, cruciale per la stabilizzazione della rete e l'ottimizzazione dei costi. Gestisce i limiti fisici di capacità ed efficienza.

**Architettura e Design:**
È progettata come una risorsa con stato (Stateful Resource). La sua operatività è limitata da: 1) **Capacità Massima** (`BATTERY_CAPACITY`), che definisce il limite superiore di immagazzinamento; 2) **Livello Attuale**, che determina l'energia disponibile per lo scaricamento.

**API Reference:**
| Metodo | Parametri | Ritorno | Descrizione |
| :--- | :--- | :--- | :--- |
| `Battery(double capacity)` | `capacity` (`double`) | - | Costruttore che stabilisce la capacità massima della batteria. |
| `charge(double quantity)` | `quantity` (`double`) | - | Simula il caricamento di energia nella batteria, aumentando l'energia immagazzinata fino alla capacità massima. |
| `discharge(double quantity)` | `quantity` (`double`) | - | Simula lo scaricamento di energia dalla batteria, diminuendo il livello disponibile. **Genera eccezioni** se la quantità richiesta eccede il livello corrente. |
| *Getters* | Nessuno | `double` | Accessori per ottenere stato fisico (es., `getLevel()`, `getFreeSpace()`). |

**Implementazione del Codice Sorgente:**

```cpp
// FILE: Battery.h
#ifndef BATTERY_H
#define BATTERY_H

#include <iostream>

/**
 * @class Battery
 * @brief Modella un sistema di accumulo energetico per lo Smart Grid Manager.
 */
class Battery 
{
private:
    double capacity;  // Capacità massima (kWh)
    double level;     // Energia attuale immagazzinata (kWh)

public:
    Battery(double initial_capacity);
    ~Battery() = default;
    
    void charge(double quantity);
    void discharge(double quantity);

    double getLevel() const { return level; }; // Livello attuale
    double getFreeSpace() const { return capacity - level; }; // Spazio rimanente
};

#endif // BATTERY_H
```