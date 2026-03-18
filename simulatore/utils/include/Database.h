#pragma once

#include <sqlite3.h>
#include <string>
#include <vector>

class Database {
private:
    sqlite3* db;

public:
    Database(const std::string& filename);
    ~Database();

    bool open();
    void close();

    bool execute(const std::string& sql);
    std::vector<std::vector<std::string>> select(const std::string& sql);

    bool createTables();
};