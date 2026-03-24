#include "utils.h"

namespace utils
{

    // devo rtitornare un puntatore ad un array di sorgenti

    void readFromDb(Database &db, std::vector<ElectricityGrid> &reti)
    {
        std::string sql;

        sql = "SELECT sorgente, ts, prezzo, prezzo_log FROM valore AS v ORDER BY sorgente, ts;";

        std::vector<std::vector<std::string>> prezzi = db.select(sql);
        if (prezzi.empty())
            std::cerr << "Errore: nessuna sorgente trovata nel database\n";

        for (const auto &row : prezzi)
        {
            int id = std::stoi(row[0]);
            double prezzo = std::stod(row[2]);
            double prezzo_log = std::stod(row[3]);

            while (static_cast<int>(reti.size()) <= id)
                reti.emplace_back();

            reti[id].setId(id);
            reti[id].addPrice(prezzo);
            reti[id].addPriceLog(prezzo_log);
        }
    }

    void insertIntoDb(Database &db, const std::string &csvPath)
    {
        std::ifstream in(csvPath);
        if (!in.is_open())
        {
            std::cerr << "Errore: impossibile aprire " << csvPath << "\n";
            return;
        }

        std::string value;
        for (int i = 0; i < 3; i++)
            in >> value;

        int ts, sorgente;
        double prezzo;

        db.execute("BEGIN TRANSACTION;");

        while (in >> ts >> sorgente >> prezzo)
        {
            std::string sql;

            sql = "INSERT OR IGNORE INTO sorgente (id) VALUES (" + std::to_string(sorgente) + ");";
            if (!db.execute(sql))
                std::cerr << "Errore: impossibile inserire la sorgente nel database\n";

            sql = "INSERT OR IGNORE INTO valore (ts, sorgente, prezzo, prezzo_log) VALUES (" +
                  std::to_string(ts) + ", " +
                  std::to_string(sorgente) + ", " +
                  std::to_string(prezzo) + ", " +
                  std::to_string(prezzo > 0 ? log(prezzo) : 0.0) + ");";
            if (!db.execute(sql))
                std::cerr << "Errore: impossibile inserire i valori nel database\n";
        }

        db.execute("COMMIT;");
    }

    double geometricMean(const ElectricityGrid &grid, int ts)
    {
        int start = ts - GEOMETRIC_WINDOW + 1;

        double logSum = 0.0;
        int count = 0;

        for (int k = start; k <= ts; ++k)
        {
            try
            {
                if (grid.getPriceByTs(k) != -1)
                {
                    logSum += grid.getPriceLogByTs(k);
                    count++;
                }
            }
            catch (const std::out_of_range &)
            {
                return -1.0; // dati insufficienti
            }
        }

        if (count == 0)
            return -1.0;

        return exp(logSum / count);

    }

}