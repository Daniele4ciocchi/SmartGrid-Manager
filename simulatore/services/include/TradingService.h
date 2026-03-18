#ifndef TRADINGSERVICE_H
#define TRADINGSERVICE_H

#include <vector>
#include "utils.h"
#include "Batteria.h"
#include "Wallet.h"


/**
 * @class TradingService
 * @brief a class that grant the selling and the buying
 */
class TradingService
{

private:
    std::vector<utils::Transazione> transazioni;

public:

    TradingService() {};
    ~TradingService() = default;

    std::vector<utils::Transazione> getTransazioni() const { return transazioni; }

    void buy(Wallet w, Batteria b, double quanti, double price);
    void sell(Wallet w, Batteria b, double quantity, double price);

};

#endif