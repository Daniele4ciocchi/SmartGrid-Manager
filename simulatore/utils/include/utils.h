#ifndef UTILS
#define UTILS

#include "Sorgente.h"
#include "ReteElettrica.h"
#include <vector>
#include <cstdio>
#include <fstream>
#include <iostream>
#include "Database.h"
#include <math.h>
#include <omp.h>
#include <string>


/**
 * @brief Dichiarazione delle funzioni di utilità per la lettura dei dati.
 * @namespace utils
 */
namespace utils
{
    /**
     * @brief Struttura per rappresentare una transazione.
     */
    struct Transazione
    {
        Sorgente *sorgente;
        double quantity;
        double price;
        double balance;
        bool in_out; // 1 = in, 0 = out
    };

    void readFromDb(Database &db, std::vector<ReteElettrica> &reti);

    void insertIntoDb(Database &db);

    void calculateGeometricMean(Database &db, int sorgente, int i, int window_size);

}
#endif 
