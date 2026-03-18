#include "Source.h"

/**
     * @brief Struttura per rappresentare una transazione.
     */
    struct Transaction
    {
        Source *source;
        double quantity;
        double price;
        double balance;
        bool in_out; // true to sell, false to buy
    };