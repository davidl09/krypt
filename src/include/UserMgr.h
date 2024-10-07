//
// Created by David Laeer on 2024-10-06.
//

#ifndef USERDATABASE_H
#define USERDATABASE_H

#include "database.h"


class UserMgr final : public Database {
public:
    UserMgr(const fs::path& dbPath, const fs::path& configPath);
    uint64_t numUsers();

    class PasswordHash;

    bool addUser(const std::string &email, const PasswordHash& password, const std::string &name, const std::string &tag, const std::string& phoneNumber);
    bool deleteUser(const std::string &email, const PasswordHash &password);
    bool validateUser(const std::string &email, const PasswordHash &password);
    db::Accounts getProfile(const std::string &email, const PasswordHash &password);

    class InvalidCredentialsException final : public DatabaseException {
    public:
        explicit InvalidCredentialsException(const std::string &message)
            : DatabaseException(message) {}

        explicit InvalidCredentialsException(const char *message)
            : DatabaseException(message) {}

    };

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

#ifndef NDEBUG
    void clearAllUsers();
#endif
};



#endif //USERDATABASE_H
