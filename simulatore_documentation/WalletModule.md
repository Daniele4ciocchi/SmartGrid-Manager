# Documentazione Accademica del Modulo Gestione Portafoglio (Wallet)
**Contesto:** Il modulo `Wallet` modella il portafoglio finanziario di un'entità partecipante nel sistema Smart Grid Manager. È responsabile della gestione dei saldi monetari derivanti dalle attività di acquisto e vendita di energia, separando il capitale disponibile (`Avaiable`) da quello messo da parte (`Set_Aside`).

**Architettura e Design:**
La classe `Wallet` opera come un gestore di stato finanziario. Il design è minimale, concentrandosi sulla protezione dell'integrità dei dati tramite operazioni atomiche (deposit/withdraw). L'interfaccia pubblica garantisce che ogni transazione sia tracciabile in termini di saldo totale e disponibile.

**API Reference:**
| Metodo | Parametri | Ritorno | Descrizione |
| :--- | :--- | :--- | :--- |
| `Wallet(double initial_balance)` | `initial_balance` (`double`) | - | Costruttore che inizializza il portafoglio con un saldo iniziale. |
| `getAvaiable()` | Nessuno | `double` | Restituisce la quantità di denaro immediatamente disponibile per le transazioni. |
| `getSet_Aside()` | Nessuno | `double` | Restituisce la somma di denaro vincolata o messa da parte. |
| `getTotalBalance()` | Nessuno | `double` | Calcola e restituisce il saldo totale del portafoglio (`Avaiable + Set_Aside`). |
| `deposit(double amount)` | `amount` (`double`) | `double` | Aumenta il saldo disponibile di `amount`. È essenziale per registrare ricavi (es. vendita). |
| `withdraw(double amount)` | `amount` (`double`) | `double` | Diminuisce il saldo disponibile, consumandolo in transazioni come gli acquisti energetici. |

**Implementazione del Codice Sorgente:**

```cpp
// FILE: Wallet.h
#ifndef WALLET_H
#define WALLET_H

#include <vector>
#include <map>
#include <iostream>
#include "Source.h"
#include "utils.h"
#include "definitions.h"

/**
 * @class Wallet
 * @brief Classe che rappresenta un portafoglio per la gestione delle transazioni di acquisto e vendita di energia.
 */
class Wallet
{
private:
    double Avaiable;  // Saldo disponibile per le transazioni
    double Set_Aside; // Saldo messo da parte

public:
    Wallet(double initial_balance) : Avaiable(initial_balance), Set_Aside(0) {};
    ~Wallet() = default;

    /**
     * @brief Ottiene il saldo disponibile nel portafoglio.
     * @return Il saldo disponibile.
     */
    double getAvaiable() { return this->Avaiable; };

    /**
     * @brief Ottiene il saldo messo da parte nel portafoglio.
     * @return Il saldo messo da parte.
     */
    double getSet_Aside() { return this->Set_Aside; };

    /**
     * @brief Ottiene il saldo totale del portafoglio (disponibile + messo da parte).
     * @return Il saldo totale.
     */
    double getTotalBalance() { return this->Avaiable + this->Set_Aside; };

    /**
     * @brief Deposita una certa quantità di denaro nel portafoglio.
     * @param amount La quantità da depositare.
     * @return Il nuovo saldo disponibile dopo il deposito.
     */    
    double deposit(double amount);

    /**
     * @brief Preleva una certa quantità di denaro dal portafoglio.
     * @param amount La quantità da prelevare.
     * @return Il nuovo saldo disponibile dopo il prelievo.
     */   
};

#endif
```