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
#include <set>


/**
 * @brief Funzione che implementa una strategia di scelta casuale per l'acquisto e la vendita di energia.
 * @param reti Vettore delle reti elettriche da cui acquistare o vendere energia.
 * @param w Puntatore al Wallet che gestisce le transazioni.
 * @param b Puntatore alla Batteria che immagazzina l'energia.
 * @param ts Puntatore al TradingService.
 * @param i Timestamp corrente.
 */
void randomChoise(std::vector<ElectricityGrid>& reti, Wallet* w, Battery* b, TradingService* ts, int j);

/**
 * @brief Funzione che implementa una strategia basata sulla media geometrica dei prezzi per l'acquisto e la vendita di energia.
 * @param reti Vettore delle reti elettriche da cui acquistare o vendere energia.
 * @param w Puntatore al Wallet che gestisce le transazioni.
 * @param b Puntatore alla Batteria che immagazzina l'energia.
 * @param ts Puntatore al TradingService.
 * @param i Timestamp corrente.
 */
void geometricChoise(std::vector<ElectricityGrid>& reti, Wallet* w, Battery* b, TradingService* ts, int j);

/**
 * @brief Funzione che implementa una strategia basata sulla media geometrica dei prezzi, con parametri di soglia più stringenti, per l'acquisto e la vendita di energia.
 * @param reti Vettore delle reti elettriche da cui acquistare o vendere energia.
 * @param w Puntatore al Wallet che gestisce le transazioni.
 * @param b Puntatore alla Batteria che immagazzina l'energia.
 * @param ts Puntatore al TradingService.
 * @param i Timestamp corrente.
 */
void smartgeometricChoise(std::vector<ElectricityGrid> &reti, Wallet *w, Battery *b, TradingService *ts, int j);


#endif 
