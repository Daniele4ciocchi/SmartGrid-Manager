#include "Wallet.h"

double Wallet::deposit(double amount)
{   
    this->Set_Aside += (amount * SET_ASIDE_PERCENTAGE) / 100.0;
    this->Avaiable += amount - (amount * SET_ASIDE_PERCENTAGE) / 100.0;
    return this->Avaiable;
}

double Wallet::withdraw(double amount)
{
    if (amount > this->Avaiable)
        throw std::out_of_range("Il prelievo supera il saldo disponibile");
    this->Avaiable -= amount;
    return this->Avaiable;
}