//
// Created by David Laeer on 2024-08-26.
//

#ifndef DATABASE_H
#define DATABASE_H



#include <ddl.h>
#include "sqlpp11/sqlpp11.h"
#include "sqlpp11/sqlite3/sqlite3.h"

#include "defs.h"

#include <filesystem>

class Database {
    public:

    Database(const fs::path& dbPath, const fs::path& configPath);
   	virtual ~Database() = default;


    class DatabaseException : public std::exception {
    public:
        explicit DatabaseException(std::string message)
            : message(std::move(message)) {}

        explicit DatabaseException(const char *message)
            : message(message) {}

        virtual ~DatabaseException() = default;
        [[nodiscard]] const char *what() const noexcept override {
            return message.c_str();
        }

    private:
        const std::string message;
    };


protected:
    sqlpp::sqlite3::connection_config config{};
    sqlpp::sqlite3::connection db{};
    std::mutex dbMutex;
};



#endif //DATABASE_H
