#ifndef RETEELETTRICA_H
#define RETEELETTRICA_H

#include "Source.h"
#include <vector>
#include <stdexcept>

/**
 * @class ElectricityGrid
 * @brief Classe derivata da Source che rappresenta una rete elettrica con una lista di prezzi.
 */
class ElectricityGrid : public Source
{
private:
    std::vector<double> prices;

public:
    ElectricityGrid() = default;
    ~ElectricityGrid() = default;

    void addPrice(double prezzo);
    double getPriceByTs(int ts);

};

#endif
