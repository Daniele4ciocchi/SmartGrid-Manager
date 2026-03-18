#include "TradingService.h"

void buy(Source *source, Wallet *w, Battery *b, double balance, double price){
    Transaction t;

    t.balance = balance;
    t.price = price;
    t.quantity = balance * price;
    t.in_out = 1; // in
    t.source = source;


}

void sell(Source *source, Wallet *w, Battery *b, double balance, double price){
    Transaction t;

    t.balance = balance;
    t.price = price;
    t.quantity = balance * price;
    t.in_out = 0; // out
    t.source = source;

}