#include "Monitor.h"

namespace monitor
{

    std::vector<double> yelds;
    
    void addYeld(double yeld)
    {

        yelds.push_back(yeld);
    }

    double getLastYeld()
    {
        if (yelds.empty())
            return 0.0;
        return yelds.back();
    }

    double getYeldByIndex(int index)
    {
        if (index < 0 || index >= (int)yelds.size())
            return 0.0;
        return yelds[index];
    }

    double calculateAverage()
    {
        double sum = 0.0;
        for (double yeld : yelds)
        {
            sum += yeld;
        }
        return yelds.empty() ? 0.0 : sum / yelds.size();
    }

    double calculateStandardDeviation()
    {
        double average = calculateAverage();
        double sum = 0.0;
        for (double yeld : yelds)
        {
            sum += (yeld - average) * (yeld - average);
        }
        return yelds.size() < 2 ? 0.0 : sqrt(sum / (yelds.size() - 1));
    }

    void insertYeldsIntoDb(Database &db)
    {
        std::string sql = "INSERT INTO esperimento DEFAULT VALUES;";
        if (!db.execute(sql))
        {
            std::cerr << "Errore: impossibile inserire un esperimento nel database\n";
            return;
        }

        sql = "SELECT id FROM esperimento ORDER BY id DESC LIMIT 1;";
        std::vector<std::vector<std::string>> results = db.select(sql);
        if (results.empty())
        {
            std::cerr << "Errore: nessun esperimento trovato nel database\n";
            return;
        }

        for (auto v : monitor::yelds)
        {
            sql = "INSERT INTO yeld (esperimento, yeld) VALUES (" +
                  results[0][0] + ", " +
                  std::to_string(v) + ");";
            if (!db.execute(sql))
                std::cerr << "Errore: impossibile inserire lo yeld nel database\n";
        }
    }
}