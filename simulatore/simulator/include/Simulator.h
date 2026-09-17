#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "Database.h"
#include "ElectricityGrid.h"
#include "Battery.h"
#include "Wallet.h"
#include "TradingService.h"
#include "utils.h"
#include "definitions.h"
#include <vector>
#include "strategy.h"
#include "Monitor.h"


class Simulator {
public:
    Simulator(Database& db, StrategyType strategy);
    void run();

private:
    Database& db;
    StrategyType strategy;
};

#endif // SIMULATOR_H
