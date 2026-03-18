#ifndef TRADINGSERVICE_H
#define TRADINGSERVICE_H

#include <vector>
#include "utils.h"


/**
 * @class TradingService
 * @brief a class that grant the selling and the buying
 */
class TradingService
{

private:
    std::vector<Transazione> transazioni;

public:

    TradingService() {};
    ~TradingService() = default;

    std::vector<Transazione> getTransazioni() const return transazioni; 

    void buy(Wallet w, Battery b, double , double price);
    void sell(Wallet w, Battery b, double quantity, double price);

};

#endif