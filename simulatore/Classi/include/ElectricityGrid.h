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
    std::vector<double> prices_log;
    // prefix sums of prices_log for O(1) range-sum queries
    std::vector<double> prices_log_prefix;
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

    // Prezzi logaritmici per la media geometrica
    void addPriceLog(double prezzo_log);
    double getPriceLogByTs(int ts) const;
    // Restituisce la somma dei log dei prezzi nell'intervallo [start, end]
    double getLogSumRange(int start, int end) const;
    // Numero di valori log caricati
    int getLogSize() const;
};

#endif
