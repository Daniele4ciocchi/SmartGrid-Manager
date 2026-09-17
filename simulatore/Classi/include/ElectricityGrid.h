#ifndef RETEELETTRICA_H
#define RETEELETTRICA_H

#include "Source.h"
#include <vector>
#include <stdexcept>

/**
 * @class ElectricityGrid
 * @brief Classe derivata da Source che rappresenta una rete elettrica con una lista di prezzi
 *        e, opzionalmente, una lista di medie geometriche associate ai timestamp.
 */
class ElectricityGrid : public Source
{
private:
    std::vector<double> prices;
    std::vector<double> prices_log; // now used as prefix sum of logs
    std::vector<int> valid_prices_count; // prefix sum of valid price counts
    int sourceId = -1;

public:
    ElectricityGrid() = default;
    ~ElectricityGrid() = default;

    // Identificativo sorgente (id nel DB)
    void setId(int id) { sourceId = id; }
    int getId() const { return sourceId; }

    // Prezzi
    void addPrice(double prezzo);
    double getPriceByTs(int ts) const;
    int getPricesSize() const { return prices.size(); }

    // Prezzi logaritmici per la media geometrica
    void addPriceLog(double prezzo_log);
    double getPriceLogSum(int start, int end) const;
    int getValidCount(int start, int end) const;
};

#endif
