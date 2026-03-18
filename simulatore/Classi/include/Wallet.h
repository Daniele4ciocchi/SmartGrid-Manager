#ifndef WALLET_H
#define WALLET_H

#include <vector>
#include <map>
#include <iostream>
#include "Sorgente.h"
#include <iostream>
#include "utils.h"
#include "definitions.h"

/**
 * @class Wallet
 * @brief Classe che rappresenta un portafoglio per la gestione delle transazioni di acquisto e vendita di energia.
 */
class Wallet
{

private:
    /** Saldo disponibile per le transazioni */
    double Avaiable;
    /** Saldo messo da parte che non si può utilizzare per acquistare*/
    double Set_Aside;

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
    double withdraw(double amount);

};

#endif