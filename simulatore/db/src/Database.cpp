#include "Database.h"
#include <iostream>

Database::Database(const std::string& filename)
    : db(nullptr)
{
    if (sqlite3_open(filename.c_str(), &db)) {
        std::cerr << "Errore apertura database: "
                  << sqlite3_errmsg(db) << std::endl;
    }
}

Database::~Database() {
    close();
}

bool Database::open() {
    return db != nullptr;
}

void Database::close() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}

bool Database::execute(const std::string& sql) {

    char* errMsg = nullptr;

    int rc = sqlite3_exec(
        db,
        sql.c_str(),
        nullptr,
        nullptr,
        &errMsg
    );

    if (rc != SQLITE_OK) {

        std::cerr << "Errore SQL: "
                  << errMsg << std::endl;

        sqlite3_free(errMsg);

        return false;
    }

    return true;
}

std::vector<std::vector<std::string>>
Database::select(const std::string& sql)
{
    std::vector<std::vector<std::string>> results;

    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(
        db,
        sql.c_str(),
        -1,
        &stmt,
        nullptr
    );

    if (rc != SQLITE_OK)
    {
        std::cerr << "Errore prepare: "
                  << sqlite3_errmsg(db) << std::endl;

        return results;
    }

    int columns = sqlite3_column_count(stmt);

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        std::vector<std::string> row;

        for (int i = 0; i < columns; i++)
        {
            const char* value =
                reinterpret_cast<const char*>(
                    sqlite3_column_text(stmt, i)
                );

            row.push_back(
                value ? value : "NULL"
            );
        }

        results.push_back(row);
    }

    if (rc != SQLITE_DONE)
    {
        std::cerr << "Errore step: "
                  << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);

    return results;
}

bool Database::createTables() {

    std::string sql = R"(

        CREATE TABLE IF NOT EXISTS sorgente (
            id INTEGER PRIMARY KEY AUTOINCREMENT
        );

        CREATE TABLE IF NOT EXISTS valore (
            ts INTEGER NOT NULL,
            sorgente INTEGER NOT NULL,
            prezzo REAL NOT NULL,
            prezzo_log REAL NOT NULL,

            PRIMARY KEY (ts, sorgente),
            FOREIGN KEY (sorgente) REFERENCES sorgente(id)
        );

    )";

    // Rimuove la tabella legacy, se esiste ancora
    execute("DROP TABLE IF EXISTS media_geometrica;");

    return execute(sql);
}