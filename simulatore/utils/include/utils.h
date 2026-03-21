#ifndef UTILS
#define UTILS

#include "Source.h"
#include "ElectricityGrid.h"
#include <vector>
#include <cstdio>
#include <fstream>
#include <iostream>
#include "Database.h"
#include <math.h>
#include <omp.h>
#include <string>
#include "definitions.h"


/**
 * @brief Dichiarazione delle funzioni di utilità per la lettura dei dati.
 * @namespace utils
 */
namespace utils
{

    void readFromDb(Database &db, std::vector<ElectricityGrid> &reti);

    // csvPath opzionale, di default usa data/datafile.csv
    void insertIntoDb(Database &db, const std::string &csvPath = "data/datafile.csv");

    // Calcola la media geometrica dei prezzi per una sorgente
    // usando i valori logaritmici già caricati in memoria
    // sulla finestra [ts - GEOMETRIC_WINDOW + 1, ts].
    // Restituisce -1.0 se non ci sono dati.
    double geometricMean(const ElectricityGrid &grid, int ts);

}
#endif 
