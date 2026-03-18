#ifndef STRATEGY
#define STRATEGY

#include "Sorgente.h"
#include "Wallet.h"
#include "ReteElettrica.h"
#include "utils.h"
#include "Batteria.h"
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
void randomChoise(ReteElettrica* r, Wallet* w, Batteria* b, int i);

void personalChoise(ReteElettrica *r, Wallet *w, Batteria *b, int i);

void geometricChoise(ReteElettrica *r, Wallet *w, Batteria *b, int i, int j);

#endif 
