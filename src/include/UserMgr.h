//
// Created by David Laeer on 2024-10-06.
//

#ifndef USERDATABASE_H
#define USERDATABASE_H

#include "database.h"
#include "ddl.h"
#include "PasswordHash.h"

class UserMgr final : public Database {
public:
    UserMgr(const fs::path& dbPath, const fs::path& configPath);
    uint64_t numUsers();

    bool addUser(const std::string &email, const std::string &name, const std::string &tag, const std::string& phoneNumber);
    bool deleteUser(const std::string &email);
    db::Accounts getProfile(const std::string &email);
    bool changeEmail(const std::string &email, const std::string &newEmail);
    bool changePassword(const std::string& email, const PasswordHash& newPassword);
    bool changePhonenumber(const std::string &email, const std::string &newPhoneNumber);

    class InvalidCredentialsException final : public DatabaseException {
    public:
        explicit InvalidCredentialsException(const std::string &message)
            : DatabaseException(message) {}

        explicit InvalidCredentialsException(const char *message)
            : DatabaseException(message) {}

    };

#ifndef NDEBUG
    void clearAllUsers();
#endif
};



#endif //USERDATABASE_H
