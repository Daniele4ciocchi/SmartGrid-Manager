#ifndef TRADINGSERVICE_H
#define TRADINGSERVICE_H

#include <vector>
#include "Transaction.h"
#include "Battery.h"
#include "Wallet.h"
#include "Source.h"
#include "ElectricityGrid.h"

/**
 * @class TradingService
 * @brief Gestisce le operazioni di acquisto e vendita registrando le transazioni.
 */
class TradingService
{
private:
    std::vector<Transaction> transactions{};
    std::map<Source *, double> sourceQuantity; 

public:
    TradingService() = default;
    ~TradingService() = default;

    std::vector<Transaction> getTransactions() const { return transactions; }

    bool canBuy(Source *source, Wallet *w, Battery *b, double balance, double price);
    bool canSell(Source *source, Wallet *w, Battery *b, double balance, double price);

    // quantity = energia (kWh) acquistata/venduta, price = prezzo per unità
    void buy(Source *source, Wallet *w, Battery *b, double balance, double price);
    void sell(Source *source, Wallet *w, Battery *b, double balance, double price);

    // ts: timestamp a cui liquidare le posizioni
    void sellAll(std::vector<ElectricityGrid> * grids, Wallet *w, Battery *b, int ts);
};

#endif