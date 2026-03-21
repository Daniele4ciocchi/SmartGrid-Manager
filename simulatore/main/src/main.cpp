#include "utils.h"
#include "definitions.h"
#include "Database.h"
#include "Simulator.h"
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char **argv)
{
    // structures declarations
    Database db("data/valori.db");
    db.createTables();

    srand(time(NULL));

    if (argc > 1 && std::string(argv[1]) == "--initdb")
    {
        utils::insertIntoDb(db, DB_PATH);
        std::cout << "Import dati completato da: " << DB_PATH << std::endl;
        return 0;
    }

    // simulatore
    if (argc > 1 && std::string(argv[1]) == "--simulator")
    {
        Simulator simulator(db);
        simulator.run();
    }

    return 0;
}
