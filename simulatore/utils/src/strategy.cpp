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
        double budget = 10;

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
    if (j < GEOMETRIC_WINDOW)
        return;

    for (ElectricityGrid &r : reti)
    {
        double price = r.getPriceByTs(j);
        if (price <= 0)
            continue;

        double geoMean = utils::geometricMean(r, j);
        if (geoMean < 0)
            continue;

        double budget = 10;

        if (price / geoMean < 0.85)
        {
            ts->buy(&r, w, b, budget, price);
        }
        else if (price / geoMean > 1.00)
        {
            ts->sell(&r, w, b, budget, price);
        }
    }
}

void smartgeometricChoise(std::vector<ElectricityGrid> &reti, Wallet *w, Battery *b, TradingService *ts, int j)
{
    if (j < GEOMETRIC_WINDOW)
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

    double budget = 70;
    const int MAX_TRANSACTIONS = 300;
    const double BUY_THRESHOLD = 0.90;
    const double SELL_THRESHOLD = 1.10;

    // Compra energia dalle reti con il rapporto prezzo/media più basso
    int buyCount = 0;
    for (const auto &[ratio, grid] : geometricValues)
    {
        if (buyCount >= MAX_TRANSACTIONS || ratio >= BUY_THRESHOLD)
            break;

        if (ts->canBuy(grid, w, b, budget, grid->getPriceByTs(j)))
            ts->buy(grid, w, b, budget, grid->getPriceByTs(j));
        buyCount++;
    }

    // Vende energia alle reti con il rapporto prezzo/media più alto (partendo dai massimi)
    int sellCount = 0;
    for (auto it = geometricValues.rbegin(); it != geometricValues.rend(); ++it)
    {
        double ratio = it->first;
        ElectricityGrid *grid = it->second;

        if (sellCount >= MAX_TRANSACTIONS || ratio <= SELL_THRESHOLD)
            break;

        if (ts->canSell(grid, w, b, budget, grid->getPriceByTs(j)))
            ts->sell(grid, w, b, budget, grid->getPriceByTs(j));
        sellCount++;
    }
}