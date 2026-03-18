#ifndef TRADINGSERVICE_H
#define TRADINGSERVICE_H

#include <vector>
#include "Transaction.h"
#include "Battery.h"
#include "Wallet.h"
#include "Source.h"

/**
 * @class TradingService
 * @brief a class that grant the selling and the buying
 */
class TradingService
{

private:
    std::vector<Transaction> transactions = {};

public:
    TradingService() {};
    ~TradingService() = default;

    std::vector<Transaction> getTransactions() const { return transactions; }

    void buy(Source *source, Wallet *w, Battery *b, double balance, double price);
    void sell(Source *source, Wallet *w, Battery *b, double balance, double price);

#endif
}