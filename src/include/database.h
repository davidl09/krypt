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

class database {
    public:
    class PasswordHash;


    explicit database(const fs::path& dbPath, const fs::path& configPath);
   	virtual ~database() = default;

    uint64_t numUsers();
    bool addUser(const std::string &email, const PasswordHash& password, const std::string &name, const std::string &tag, const std::string& phoneNumber);
    bool deleteUser(const std::string &email, const PasswordHash &password);
    bool validateUser(const std::string &email, const PasswordHash &password);

    db::Accounts getProfile(const std::string &email, const PasswordHash &password);


#ifndef NDEBUG
    void clearAllUsers();
#endif

    class PasswordHash {
    public:
        PasswordHash(const std::string&);
        PasswordHash(std::string&& str);
        ~PasswordHash() = default;
        operator std::string() const;
    private:
        static std::string hashStringToSha256Hex(std::string_view input);
        std::string passwordHash;
    };

    class DatabaseException : public std::exception {
    public:
        explicit DatabaseException(const std::string &message)
            : message(message) {}

        explicit DatabaseException(const char *message)
            : message(message) {}

        virtual ~DatabaseException() = default;
        [[nodiscard]] const char *what() const noexcept {
            return message.c_str();
        }

    private:
        const std::string message;
    };

    class InvalidCredentialsException final : public DatabaseException {
    public:
        explicit InvalidCredentialsException(const std::string &message)
            : DatabaseException(message) {}

        explicit InvalidCredentialsException(const char *message)
            : DatabaseException(message) {}

    };
private:
    sqlpp::sqlite3::connection_config config{};
    sqlpp::sqlite3::connection db{};
    std::mutex dbMutex;
};



#endif //DATABASE_H
