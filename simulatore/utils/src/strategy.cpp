#include "strategy.h"
#include <stdexcept>
#include <algorithm>

void randomChoise(std::vector<ElectricityGrid> &reti, Wallet *w, Battery *b, TradingService *ts, int j)
{
    thread_local std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<int> action(0, 2); // 0 = niente, 1 = buy, 2 = sell

    for (ElectricityGrid &r : reti)
    {
        double price = r.getPriceByTs(j);
        if (price == -1)
            continue;
        double budget = BUDGET;

        int choice = action(gen);

        if (choice == 1 && ts->canBuy(&r, w, b, budget, price))
        {
            ts->buy(&r, w, b, budget, price);
        }
        else if (choice == 2 && ts->canSell(&r, w, b, budget, price))
        {
            ts->sell(&r, w, b, budget, price);
        }
    }
}

void geometricChoise(std::vector<ElectricityGrid> &reti, Wallet *w, Battery *b, TradingService *ts, int j)
{
    if (j <= GEOMETRIC_WINDOW)
        return;

    for (ElectricityGrid &r : reti)
    {
        double price = r.getPriceByTs(j);
        if (price == -1)
            continue;

        double geoMean = utils::geometricMean(r, j);
        if (geoMean < 0)
            continue;


        if (price / geoMean < BUY_THRESHOLD)
        {
            ts->buy(&r, w, b, BUDGET, price);
        }
        else if (price / geoMean > SELL_THRESHOLD)
        {
            ts->sell(&r, w, b, BUDGET, price);
        }
    }
}

void smartgeometricChoise(std::vector<ElectricityGrid> &reti, Wallet *w, Battery *b, TradingService *ts, int j)
{
    if (j <= GEOMETRIC_WINDOW)
        return;

    std::set<std::pair<double, ElectricityGrid *>> geometricValues;

    for (ElectricityGrid &r : reti)
    {
        double price = r.getPriceByTs(j);
        if (price == -1)
            continue;

        double geoMean = utils::geometricMean(r, j);
        if (geoMean < 0)
            continue;

        geometricValues.insert({price / geoMean, &r});
    }

  
    int soglia = 0;
    // Compra energia dalle reti con il rapporto prezzo/media più basso

    for (const auto &[ratio, grid] : geometricValues)
    {
        if (ratio >= BUY_THRESHOLD || soglia >= MAX_BUY)
            break;

        if (ts->canBuy(grid, w, b, BUDGET, grid->getPriceByTs(j)))
            ts->buy(grid, w, b, BUDGET, grid->getPriceByTs(j));
        soglia++;
    }

    soglia = 0;
    // Vende energia alle reti con il rapporto prezzo/media più alto (partendo dai massimi)
    for (auto it = geometricValues.rbegin(); it != geometricValues.rend(); ++it)
    {
        double ratio = it->first;
        ElectricityGrid *grid = it->second;

        if (ratio <= SELL_THRESHOLD || soglia >= MAX_SELL)
            break;

        if (ts->canSell(grid, w, b, BUDGET, grid->getPriceByTs(j)))
            ts->sell(grid, w, b, BUDGET, grid->getPriceByTs(j));
        soglia++;
    }
}