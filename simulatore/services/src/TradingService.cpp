#include "TradingService.h"

void buy(Wallet w, Batteria b, double balance, double price){
    utils::Transazione t;
    t.batteria = b;
    t.quantita = balance;
    t.prezzo = price;
    t.tipo = "buy";
    transazioni.push_back(t);
}

void sell(Wallet w, Batteria b, double balance, double price){
    utils::Transazione t;
    t.batteria = b;
    t.quantita = quantity;
    t.prezzo = price;
    t.tipo = "sell";
    transazioni.push_back(t);
}