#include "ElectricityGrid.h"

/**
 * @brief Aggiunge un prezzo alla lista dei prezzi della rete elettrica.
 * @param prezzo Il prezzo da aggiungere.
 */
void ElectricityGrid::addPrice(double prezzo)
{
    prices.push_back(prezzo);
}

/**
 * @brief Restituisce il prezzo corrispondente a un timestamp specifico.
 * @param ts Il timestamp per cui si desidera ottenere il prezzo.
 * @return Il prezzo corrispondente al timestamp.
 * @throws std::out_of_range se il timestamp è fuori dai limiti.
 */
double ElectricityGrid::getPriceByTs(int ts) const
{
    if (ts < 0 || ts >= static_cast<int>(prices.size()))
    {
        throw std::out_of_range("Timestamp fuori dai limiti");
    }
    return prices[ts];
}

void ElectricityGrid::addPriceLog(double prezzo_log)
{
    prices_log.push_back(prezzo_log);
}

double ElectricityGrid::getPriceLogByTs(int ts) const
{
    if (ts < 0 || ts >= static_cast<int>(prices_log.size()))
    {
        return -1.0; // nessun dato disponibile per questo timestamp
    }
    return prices_log[ts];
}


