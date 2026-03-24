#include "TradingService.h"

bool TradingService::canBuy(Source *source, Wallet *w, Battery *b, double balance, double price)
{
    double quantity = balance / price;
    return w->getAvaiable() >= balance && b->getFreeSpace() >= quantity;
}

bool TradingService::canSell(Source *source, Wallet *w, Battery *b, double balance, double price)
{
    double quantity = balance / price;
    return b->getLevel() >= quantity;
}

void TradingService::buy(Source *source, Wallet *w, Battery *b, double balance, double price)
{
    double quantity = balance / price;

    if (!canBuy(source, w, b, balance, price))
        return;

    Transaction t;
    t.balance = balance;
    t.price = price;
    t.quantity = quantity;
    t.in_out = false; // in
    t.source = source;

    try
    {
        w->withdraw(balance);
        b->charge(quantity);

        transactions.push_back(t);
        sourceQuantity[source] += quantity;
    }
    catch (const std::out_of_range &)
    {
        return;
    }
}

void TradingService::sell(Source *source, Wallet *w, Battery *b, double balance, double price)
{
    double quantity = balance / price;

    if ( b->getLevel() < quantity)
        return;

    Transaction t;
    t.balance = balance;
    t.price = price;
    t.quantity = quantity;
    t.in_out = true; // out
    t.source = source;

    try
    {
        w->deposit(balance);
        b->discharge(quantity);

        transactions.push_back(t);
        sourceQuantity[source] -= quantity;
    }
    catch (const std::out_of_range &)
    {
        return;
    }
}

void TradingService::sellAll(std::vector<ElectricityGrid> *grids, Wallet *w, Battery *b, int ts)
{
    // grids è un puntatore a std::vector<ElectricityGrid>, devo dereferenziarlo
    for (ElectricityGrid &grid : *grids)
    {
        Source *source = &grid;
        double quantity = sourceQuantity[source];
        if (quantity > 0)
        {
            while (grid.getPriceByTs(ts) <= 0)
                ts--;
            sell(source, w, b, quantity * grid.getPriceByTs(ts), grid.getPriceByTs(ts));
            sourceQuantity[source] = 0;
        }
    }
}