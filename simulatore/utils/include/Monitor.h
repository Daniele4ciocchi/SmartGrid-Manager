#ifndef MONITOR_H
#define MONITOR_H

#include <vector>
#include <cmath>
#include <iostream>
#include "Database.h"

/**
 * @namespace monitor
 * @brief Namespace che contiene funzioni per monitorare e calcolare statistiche sui rendimenti.
 */
namespace monitor
{
    /**
     * @brief Vettore che memorizza i rendimenti.
     */
    extern std::vector<double> yelds;

    /**
     * @brief Aggiunge un rendimento al vettore dei rendimenti.
     * @param yeld Il rendimento da aggiungere.
     */
    void addYeld(double yeld);

    /**
     * @brief restituisce lo yeld da un indice specifico.
     * @param index L'indice da cui ottenere lo yeld.
     */
    double getYeldByIndex(int index);

    /**
     * @brief Restituisce l'ultimo rendimento aggiunto.
     * @return L'ultimo rendimento.
     */
    double getLastYeld();

    /**
     * @brief Calcola la media dei rendimenti.
     * @return La media dei rendimenti.
     */
    double calculateAverage();

    /**
     * @brief Calcola la deviazione standard dei rendimenti.
     * @return La deviazione standard dei rendimenti.
     */
    double calculateStandardDeviation();

#endif
}