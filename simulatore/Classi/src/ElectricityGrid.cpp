#include "ElectricityGrid.h"


/**
 * @brief Aggiunge un prezzo alla lista dei prezzi della rete elettrica.
 * @param prezzo Il prezzo da aggiungere.
 */
void ElectricityGrid::addPrice(double prezzo){
    prices.push_back(prezzo);
}

/**
 * @brief Restituisce il prezzo corrispondente a un timestamp specifico.
 * @param ts Il timestamp per cui si desidera ottenere il prezzo.
 * @return Il prezzo corrispondente al timestamp, o 0.0 se il timestamp è fuori dai limiti.
 */
double ElectricityGrid::getPriceByTs(int ts) {
    if (ts < 0 || ts >= prices.size()) { throw std::out_of_range("Timestamp fuori dai limiti"); }
    return prices[ts];
}

