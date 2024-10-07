//
// Created by David Laeer on 2024-08-26.
//


#include "database.h"
#include "defs.h"

#include "sqlpp11/sqlpp11.h"
#include "sqlpp11/sqlite3/sqlite3.h"

#include <fstream>
#include <string>



Database::Database(const fs::path& dbPath, const fs::path& configPath = "")
    : config(dbPath, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE), db(config) {

    if (std::ifstream cfgCmd{configPath}; cfgCmd) {
        std::string command((std::istreambuf_iterator(cfgCmd)),
                             std::istreambuf_iterator<char>());
        db.execute(command);
    } else {
        throw std::runtime_error("Failed to open config file");
    }
}


