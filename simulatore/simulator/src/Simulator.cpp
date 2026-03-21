#include "Simulator.h"
#include <omp.h>

Simulator::Simulator(Database &db) : db(db) {}

void Simulator::run()
{
    std::vector<ElectricityGrid> reti;
    utils::readFromDb(db, reti);

// parallelizzo sul ciclo esterno (ogni i è indipendente)
#pragma omp parallel
    {

#pragma omp for schedule(static)
        for (int i = GEOMETRIC_WINDOW; i <= TIMESTAMP - WINDOW_SIZE; ++i)
        {

            Battery b(BATTERY_CAPACITY);
            Wallet w(WALLET_INITIAL_BALANCE);
            TradingService tradingService;

            // ciclo interno: finestra di WINDOW_SIZE (300) ts a partire da i
            for (int j = i; j < i + WINDOW_SIZE; ++j)
            {
                for (ElectricityGrid &r : reti)
                {
                    randomChoise(&r, &w, &b, &tradingService, j);
                }
            }

            monitor::addYeld((w.getTotalBalance() - WALLET_INITIAL_BALANCE) / WALLET_INITIAL_BALANCE);
        }
    }

    std::cout << "media " << monitor::calculateAverage() << std::endl;
    std::cout << "deviazione standard " << monitor::calculateStandardDeviation() << std::endl;
}
