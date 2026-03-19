#include "TradingService.h"

void buy(Source *source, Wallet *w, Battery *b, double balance, double price){
    Transaction t;

    t.balance = balance;
    t.price = price;
    t.quantity = balance * price;
    t.in_out = 0; // in
    t.source = source;

    w->withdraw(t.quantity);
    b->charge(t.quantity);
}

void sell(Source *source, Wallet *w, Battery *b, double balance, double price){
    Transaction t;

    t.balance = balance;
    t.price = price;
    t.quantity = balance * price;
    t.in_out = 1; // out
    t.source = source;

    w->deposit(t.quantity);
    b->discharge(t.quantity);
}