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


/**
 * @brief Dichiarazione delle funzioni di utilità per la lettura dei dati.
 * @namespace utils
 */
namespace utils
{

    void readFromDb(Database &db, std::vector<ElectricityGrid> &reti);

    void insertIntoDb(Database &db);

    void calculateGeometricMean(Database &db, int sorgente, int i, int window_size);

}
#endif 
