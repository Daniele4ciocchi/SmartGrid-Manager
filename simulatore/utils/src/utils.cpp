#include "utils.h"

namespace utils
{

    // devo rtitornare un puntatore ad un array di sorgenti

    void readFromDb(Database &db, std::vector<ReteElettrica> &reti)
    {
        std::string sql;

        sql = "SELECT sorgente, ts, prezzo FROM valore AS v ORDER BY sorgente, ts;";

        std::vector<std::vector<std::string>> prezzi = db.select(sql);
        if (prezzi.empty())
            std::cerr << "Errore: nessuna sorgente trovata nel database\n";

        sql = "SELECT * FROM media_geometrica AS m ORDER BY sorgente, da_ts;";
        std::vector<std::vector<std::string>> medie = db.select(sql);
        if (medie.empty())
            std::cerr << "Errore: nessuna media geometrica trovata nel database\n";

        for (const auto &row : prezzi)
        {
            int id = std::stoi(row[0]);
            int ts = std::stoi(row[1]);
            double prezzo = std::stod(row[2]);

            while ((int)reti.size() <= id)
                reti.emplace_back();

            reti[id].addPrezzo(prezzo);
        }
        
        for (const auto &row : medie)
        {
            int id = std::stoi(row[0]);
            int ts = std::stoi(row[2]);
            double media = std::stod(row[3]);


            reti[id].addMedia(media);
        }
        
    }

    void insertIntoDb(Database &db)
    {
        std::ifstream in("data/datafile.csv");
        // ifstream in("data/datamio.csv");
        if (!in.is_open())
        {
            std::cerr << "Errore: impossibile aprire data/datafile.csv\n";
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

    void calculateGeometricMean(Database &db, int sorgente, int i, int window_size)
    {
        double geometric_mean = 0.0;
        std::string sql = "SELECT prezzo_log FROM valore WHERE sorgente = " + std::to_string(sorgente) +
                          " AND ts >= " + std::to_string(i) +
                          " AND ts < " + std::to_string(i + window_size) +
                          " AND prezzo > 0;";

        std::vector<std::vector<std::string>> results = db.select(sql);
        if (!results.empty())
        {
            double sum = 0.0;
            int count = 0;
            for (const auto &row : results)
            {
                sum += std::stod(row[0]);
                count++;
            }

            // double geometric_mean = exp(sum) / count;
            geometric_mean = exp(sum / count);
        }

        if (sorgente == 1)
            std::cout << "Sorgente: " << sorgente << ", da_ts: " << i << ", a_ts: " << i + window_size << ", media geometrica: " << geometric_mean << std::endl;

        sql = "INSERT OR REPLACE INTO media_geometrica (sorgente, da_ts, a_ts, media) VALUES (" +
              std::to_string(sorgente) + ", " +
              std::to_string(i) + ", " +
              std::to_string(i + window_size) + ", " +
              std::to_string(geometric_mean) + ");";
        if (!db.execute(sql))
        {
            std::cerr << "Errore: impossibile inserire la media geometrica nel database\n";
        }
    }

}