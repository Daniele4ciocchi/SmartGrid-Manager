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
        return -1.0;
    }
    return prices[ts];
}

void ElectricityGrid::addPriceLog(double prezzo_log)
{
    double last_log = prices_log.empty() ? 0.0 : prices_log.back();
    prices_log.push_back(last_log + prezzo_log);
    
    int last_count = valid_prices_count.empty() ? 0 : valid_prices_count.back();
    // prezzo_log non è 0 se prezzo > 0, oppure log(prezzo). Wait, checking price > 0 can be done by checking if we had price>0
    // Actually, addPrice is always called just before addPriceLog, so we can use prices.back()
    int is_valid = (!prices.empty() && prices.back() > 0.0) ? 1 : 0;
    valid_prices_count.push_back(last_count + is_valid);
}

double ElectricityGrid::getPriceLogSum(int start, int end) const
{
    if (start >= end) return 0.0;
    double logSum = 0.0;
    if (start == 0) {
        logSum = prices_log[end - 1];
    } else {
        logSum = prices_log[end - 1] - prices_log[start - 1];
    }
    return logSum;
}

int ElectricityGrid::getValidCount(int start, int end) const
{
    if (start >= end) return 0;
    int count = 0;
    if (start == 0) {
        count = valid_prices_count[end - 1];
    } else {
        count = valid_prices_count[end - 1] - valid_prices_count[start - 1];
    }
    return count;
}


