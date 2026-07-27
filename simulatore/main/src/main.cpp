#include "utils.h"
#include "definitions.h"
#include "Database.h"
#include "Simulator.h"
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

// Definizione ed inizializzazione coi valori di default
int WINDOW_SIZE = 300;
int GEOMETRIC_WINDOW = 20;
double WALLET_INITIAL_BALANCE = 1000.0;
double SET_ASIDE_PERCENTAGE = 20.0;
double BATTERY_CAPACITY = 1000.0;
double BUDGET = 10.0;
double BUY_THRESHOLD = 0.80;
double SELL_THRESHOLD = 1.00;
int MAX_BUY = 200; // massimo numero di acquisti per timestamp
int MAX_SELL = 200; // massimo numero di vendite per timestamp

int main(int argc, char **argv)
{
    // Parse argomenti opzionali per tuning dei test
    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg == "--budget" && i + 1 < argc) BUDGET = std::stod(argv[++i]);
        else if (arg == "--buy" && i + 1 < argc) BUY_THRESHOLD = std::stod(argv[++i]);
        else if (arg == "--sell" && i + 1 < argc) SELL_THRESHOLD = std::stod(argv[++i]);
        else if (arg == "--geom" && i + 1 < argc) GEOMETRIC_WINDOW = std::stoi(argv[++i]);
        else if (arg == "--window" && i + 1 < argc) WINDOW_SIZE = std::stoi(argv[++i]);
        else if (arg == "--wallet" && i + 1 < argc) WALLET_INITIAL_BALANCE = std::stod(argv[++i]);
        else if (arg == "--set-aside" && i + 1 < argc) SET_ASIDE_PERCENTAGE = std::stod(argv[++i]);
    }

    // structures declarations
    Database db("data/valori.db");
    db.createTables();

    srand(time(NULL));

    // Controllo argomenti di azione (devono essere iterati in maniera flessibile o a fine stringa)
    bool runInit = false;
    bool runSimulator = false;

    for (int i = 1; i < argc; ++i)
    {
        if (std::string(argv[i]) == "--initdb") runInit = true;
        if (std::string(argv[i]) == "--simulator") runSimulator = true;
    }

    if (runInit)
    {
        utils::insertIntoDb(db, DB_PATH);
        std::cout << "Import dati completato da: " << DB_PATH << std::endl;
        return 0;
    }

    // simulatore
    if (runSimulator)
    {
        Simulator simulator(db);
        simulator.run();
    }

    return 0;
}
