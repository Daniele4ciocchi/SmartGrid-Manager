#include "strategy.h"

void randomChoise(ElectricityGrid *r, Wallet *w, Battery *b, TradingService *ts, int j)
{
    // generatore casuale per-thread (sicuro con OpenMP)
    thread_local std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<int> coin(0, 1); // 0 = buy, 1 = sell

    double price = r->getPriceByTs(j);

    if (coin(gen) == 0)
    {
        // buy
        double quantity = std::min({b->getFreeSpace(), w->getAvaiable() / price, 10.0}); // limita la quantità massima a 10 kWh
        if (quantity > 0)
        {
            ts->buy(r, w, b, quantity, price);
        }
    }
    else
    {
        // sell
        double quantity = std::min(b->getLevel(), 10.0); // limita la quantità massima a 10 kWh
        if (quantity > 0)
        {
            ts->sell(r, w, b, quantity, price);
        }
    }
}
