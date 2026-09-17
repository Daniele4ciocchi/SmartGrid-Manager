# Documentazione Accademica del Modulo Transazioni Commerciali (TradingService)
**Contesto:** `TradingService` incapsula la logica complessa di interscambio energetico tra i componenti dello Smart Grid Manager (Sorgenti, Batteria e Wallet). È il *mediatore* che assicura che tutte le transazioni rispettino vincoli fisici (capacità batteria) ed economici (saldo wallet).

**Architettura e Design:**
Questo modulo è un **Service Layer**. Mantiene una traccia dettagliata di ogni singola transazione (`Transaction`) e della quantità totale scambiata da/verso ogni sorgente. Le funzioni `canBuy` e `canSell` implementano la logica di validazione, prevenendo stati inconsistenti del sistema (es. tentativo di prelevare energia che non esiste).

**API Reference:**
| Metodo | Parametri | Ritorno | Descrizione |
| :--- | :--- | :--- | :--- |
| `canBuy(...)` | Sorgente, Wallet, Batteria, Budget, Prezzo | `bool` | Verifica se è finanziariamente e fisicamente possibile acquistare energia. |
| `buy(...)` | Sorgente, Wallet, Batteria, Budget, Prezzo | `void` | Esegue l'acquisto: decrementa il wallet e incrementa la carica della batteria. |
| `canSell(...)` | Sorgente, Wallet, Batteria, Budget, Prezzo | `bool` | Verifica se è finanziariamente e fisicamente possibile vendere energia. |
| `sell(...)` | Sorgente, Wallet, Batteria, Budget, Prezzo | `void` | Esegue la vendita: incrementa il wallet e decrementa lo scarico della batteria. |
| `sellAll(grids, w, b, ts)` | Vetore Rete, Wallet, Batteria, Timestamp | `void` | Funzione di liquidazione periodica: vende l'energia residua delle sorgenti al momento finale della finestra simulativa. |

**Implementazione del Codice Sorgente:**

```cpp
// FILE: TradingService.h
#ifndef TRADINGSERVICE_H
#define TRADINGSERVICE_H

#include <vector>
#include <map>
#include "Transaction.h" 
#include "Battery.h"
#include "Wallet.h"
#include "Source.h"
#include "ElectricityGrid.h"

/**
 * @class TradingService
 * @brief Gestisce le operazioni di acquisto e vendita registrando le transazioni nel sistema Smart Grid Manager.
 */
class TradingService
{
private:
    std::vector<Transaction> transactions{}; // Log delle transazioni storiche
    std::map<Source*, double> sourceQuantity; // Quantità totale scambiata da ogni sorgente

public:
    TradingService() = default;
    ~TradingService() = default;

    // ... Getter e metodi di controllo (canBuy, canSell)
    bool canBuy(Source *source, Wallet *w, Battery *b, double balance, double price);
    bool canSell(Source *source, Wallet *w, Battery *b, double balance, double price);

    // Funzioni operative (buy/sell): modificano lo stato di Wallet e Battery.
    void buy(Source *source, Wallet *w, Battery *b, double balance, double price);
    void sell(Source *source, Wallet *w, Battery *b, double balance, double price);

    // Liquidazione: vende tutto il surplus accumulato al momento finale (ts).
    void sellAll(std::vector<ElectricityGrid> * grids, Wallet *w, Battery *b, int ts);
};

#endif
```