#include "TradingService.h"

void TradingService::buy(Source *source, Wallet *w, Battery *b, double quantity, double price)
{
    Transaction t;

    double cost = quantity * price;

    t.balance = w->getTotalBalance();
    t.price = price;
    t.quantity = quantity;
    t.in_out = false; // in
    t.source = source;

    w->withdraw(cost);
    b->charge(quantity);

    transactions.push_back(t);
}

void TradingService::sell(Source *source, Wallet *w, Battery *b, double quantity, double price)
{
    Transaction t;

    double revenue = quantity * price;

    t.balance = w->getTotalBalance();
    t.price = price;
    t.quantity = quantity;
    t.in_out = true; // out
    t.source = source;

    w->deposit(revenue);
    b->discharge(quantity);

    transactions.push_back(t);
}