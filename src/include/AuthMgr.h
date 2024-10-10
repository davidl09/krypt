//
// Created by David Laeer on 2024-10-10.
//

#ifndef AUTHMGR_H
#define AUTHMGR_H

#include <database.h>
#include <TokenMgr.h>

#include "defs.h"
#include "PasswordHash.h"

class AuthMgr final : public Database {
public:
    using token_type = uint64_t;

    enum class result_type {
        OK,
        BAD_PSWD,
        NO_ACCT
    };
    struct AuthReturnType {
        token_type token;
        result_type result;
    };


    AuthMgr();
    ~AuthMgr() override;
    bool addUser(const std::string& email, const PasswordHash& password);
    AuthReturnType authenticateUser(const std::string &email, const PasswordHash &password, Clock::duration lifetime);
    bool authenticateUser(token_type token);




private:
    TokenMgr<uint64_t> tokenMgr;
};


#endif //AUTHMGR_H
