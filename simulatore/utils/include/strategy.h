#ifndef STRATEGY
#define STRATEGY

#include "Source.h"
#include "Wallet.h"
#include "ElectricityGrid.h"
#include "utils.h"
#include "Battery.h"
#include "TradingService.h"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <random>
#include <cmath>
#include <limits>
#include <iomanip>



/**
 * @brief Funzione che implementa una strategia di scelta casuale per l'acquisto e la vendita di energia.
 * @param r Puntatore alla ReteElettrica da cui acquistare o vendere energia.
 * @param w Puntatore al Wallet che gestisce le transazioni.
 * @param b Puntatore alla Batteria che immagazzina l'energia.
 * @param i Timestamp corrente.
 */
void randomChoise(ElectricityGrid* r, Wallet* w, Battery* b, TradingService* ts, int i);

#endif 
