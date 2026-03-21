#ifndef TRADINGSERVICE_H
#define TRADINGSERVICE_H

#include <vector>
#include "Transaction.h"
#include "Battery.h"
#include "Wallet.h"
#include "Source.h"

/**
 * @class TradingService
 * @brief Gestisce le operazioni di acquisto e vendita registrando le transazioni.
 */
class TradingService
{
private:
    std::vector<Transaction> transactions{};

public:
    TradingService() = default;
    ~TradingService() = default;

    std::vector<Transaction> getTransactions() const { return transactions; }

    // quantity = energia (kWh) acquistata/venduta, price = prezzo per unità
    void buy(Source *source, Wallet *w, Battery *b, double quantity, double price);
    void sell(Source *source, Wallet *w, Battery *b, double quantity, double price);
};

#endif