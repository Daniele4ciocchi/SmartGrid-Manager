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
        // Partiamo da GEOMETRIC_WINDOW timestamp indietro
        int start = std::max(0, ts - GEOMETRIC_WINDOW);
        int end = std::min(ts, grid.getPricesSize());

        if (start >= end)
            return -1.0;

        int count = grid.getValidCount(start, end);
        if (count == 0)
            return -1.0; // dati insufficienti

        double logSum = grid.getPriceLogSum(start, end);
        return exp(logSum / count);
    }

    void logExperiment(const std::string &strategyName, double averageYield, double standardDeviation, const std::string &outputFile)
    {
        bool fileExists = false;
        std::ifstream f(outputFile.c_str());
        if (f.good())
        {
            fileExists = true;
        }
        f.close();

        std::ofstream out(outputFile, std::ios_base::app);
        if (!out.is_open())
        {
            std::cerr << "Errore: impossibile aprire " << outputFile << " per scrivere i risultati\n";
            return;
        }

        if (!fileExists)
        {
            // Scrive l'header se il file non esiste
            out << "Strategia,Finestra Geometrica,Finestra Simulazione,Budget,Initial Balance,Set Aside %,Buy Threshold,Sell Threshold,Average Yield (%),Standard Deviation (%),Guadagno Finale Medio\n";
        }

        out << strategyName << ","
            << GEOMETRIC_WINDOW << ","
            << WINDOW_SIZE << ","
            << BUDGET << ","
            << WALLET_INITIAL_BALANCE << ","
            << SET_ASIDE_PERCENTAGE << ","
            << BUY_THRESHOLD << ","
            << SELL_THRESHOLD << ","
            << averageYield * 100 << ","
            << standardDeviation * 100 << ","
            << WALLET_INITIAL_BALANCE * averageYield << "\n";

        out.close();
    }

}