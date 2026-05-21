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

            try
            {
                // ciclo interno: finestra di WINDOW_SIZE (300) ts a partire da i
                for (int j = i; j < i + WINDOW_SIZE; ++j)
                {
                    // randomChoise(reti, &w, &b, &tradingService, j);
                    // geometricChoise(reti, &w, &b, &tradingService, j);
                    smartgeometricChoise(reti, &w, &b, &tradingService, j);
                }

                // liquida tutte le posizioni al timestamp finale della finestra
                tradingService.sellAll(&reti, &w, &b, i + WINDOW_SIZE - 1);

#pragma omp critical
                monitor::addYeld((w.getTotalBalance() - WALLET_INITIAL_BALANCE) / WALLET_INITIAL_BALANCE);
                // std::cout << "Finestra " << i << "-" << i + WINDOW_SIZE - 1 << ": balance = " << w.getTotalBalance() << "€, yeld = " << monitor::getLastYeld() * 100 << "%\n";
                // std::cout << "Finestra " << i << "-" << i + WINDOW_SIZE - 1 << ": yeld = " << monitor::getLastYeld() * 100 << "%\n";
            }
            catch (const std::out_of_range &)
            {
                // Qualsiasi eccezione dalla batteria in questa finestra: salta questo i
                continue; // passa al prossimo i del for omp
            }
        }
    }
    double avgYield = monitor::calculateAverage();
    double stdDev = monitor::calculateStandardDeviation();

    std::cout << "budget: " << BUDGET << std::endl;
    std::cout << "buy threshold: " << BUY_THRESHOLD << std::endl;
    std::cout << "sell threshold: " << SELL_THRESHOLD << std::endl;
    std::cout << "set aside: " << SET_ASIDE_PERCENTAGE << "%" << std::endl;
    std::cout << "geometric window: " << GEOMETRIC_WINDOW << std::endl;
    std::cout << "media yeld: " << avgYield * 100 << "%" << std::endl;
    std::cout << "deviazione standard yeld: " << stdDev * 100 << "%" << std::endl;
    std::cout << "range: " << (avgYield - stdDev) * 100 << "% - " << (avgYield + stdDev) * 100 << "%" << std::endl;

    // Registra i dati dell'esperimento
    // Nota: modificare il nome della strategia in base a quella abilitata nel ciclo sopra
    utils::logExperiment("smartGeometricChoise", avgYield, stdDev);
}
