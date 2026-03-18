#include "Rinnovabile.h"
#include "Batteria.h"
#include "Consumatore.h"
#include "ReteElettrica.h"
#include "utils.h"
#include "Database.h"
#include "strategy.h"
#include "Wallet.h"
#include "Monitor.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <omp.h>

#define TIMESTAMP 5966
#define WINDOW_SIZE 300
#define GEOMETRIC_WINDOW 10
#define WALLET_INITIAL_BALANCE 1000.0

int main(int argc, char **argv)
{
    // structures declarations
    Database db("data/valori.db");
    db.createTables();

    srand(time(NULL));

    // init the db
    if (argc > 1 && std::string(argv[1]) == "--initdb")
    {
        // Popola il database con i dati dal file CSV
        utils::insertIntoDb(db);

        db.execute("BEGIN TRANSACTION;");

        std::cout << "Database inizializzato con i dati del file CSV." << std::endl;
        for (int i = 0; i <= TIMESTAMP - GEOMETRIC_WINDOW; i++)
        {
            std::string sql = "SELECT * FROM sorgente;";
            std::vector<std::vector<std::string>> results = db.select(sql);
            if (results.empty())
            {
                std::cerr << "Errore: nessuna sorgente trovata nel database\n";
            }

            for (const auto &row : results)
            {

                int sorgente = std::stoi(row[0]);
                utils::calculateGeometricMean(db, sorgente, i, GEOMETRIC_WINDOW);
            }

            if (i % 100 == 0)
                std::cout << i << std::endl;
        }
        db.execute("COMMIT;");
        return 0;
    }

    // simulatore
    if (argc > 1 && std::string(argv[1]) == "--simulator")
    {
        
        std::vector<ReteElettrica> reti;
        utils::readFromDb(db, reti);

#pragma omp parallel for
        for (int i = 0; i <= TIMESTAMP - WINDOW_SIZE; i++)
        {
            Batteria b(1000);
            Wallet w(WALLET_INITIAL_BALANCE);
            

            for (int j = i; j < i + WINDOW_SIZE; j++)
            {

                for (ReteElettrica &r : reti)
                {

                    if (r.getPrezzoByTs(j) == -1)
                        continue;

                    if (((j - i + 1) % WINDOW_SIZE == 0 || j == TIMESTAMP) && j != 0)
                    {
                        utils::Transazione t = w.sellAll(&r, r.getPrezzoByTs(j));
                        b.scarica(t.corrente);
                    }

                    else
                    {
                        // randomChoise(&r, &w, &b, j);
                        // personalChoise(&r, &w, &b, j);
                        geometricChoise(&r, &w, &b, i, j);
                    }
                }
            }

#pragma omp critical
            monitor::addYeld((w.getAvaiable() + w.getSet_Aside() - WALLET_INITIAL_BALANCE) / WALLET_INITIAL_BALANCE);
            // std::cout <<  monitor::getLastYeld()*100 << "%" << std::endl;
        }
        std::cout << "Media:" << std::fixed << std::setprecision(18) << monitor::calculateAverage() * 100 << "%" << std::endl;
        std::cout << "Deviazione Standard:" << std::fixed << std::setprecision(18) << monitor::calculateStandardDeviation() * 100 << "%" << std::endl;
    }

    return 0;
}
