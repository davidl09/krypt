//
// Created by David Laeer on 2024-10-10.
//

#include <AuthMgr.h>
#include "settings.h"
#include "authmgr.h"
#include "PasswordHash.h"

using namespace std;
using namespace auth;
using namespace sqlpp;

AuthMgr::AuthMgr()
    : Database(AUTHMGR_DB, AUTHMGR_SCHEMA) {
}


AuthMgr::~AuthMgr() = default;

bool AuthMgr::addUser(const string &email, const PasswordHash &password) {

    Credentials creds;
    scoped_lock lock{dbMutex};

    if (const auto result = db(select(creds.email).from(creds).where(creds.email.like(email))); not result.empty()) {
        return false;
    }

    db(insert_into(creds).set(creds.email = email, creds.passwordHash = string{password}));

    return true;
}

AuthMgr::AuthReturnType AuthMgr::authenticateUser(const string &email, const  PasswordHash& password, const Clock::duration lifetime) {
    using namespace auth;
    using namespace sqlpp;
    Credentials creds;

    dbMutex.lock();
    auto result = db(select(creds.email, creds.passwordHash).from(creds).where(creds.email == email));
    dbMutex.unlock();

    const string hashedPassword = password;
    size_t userCount = 0;
    for (const auto &user : result) {
        ++userCount;
        if (user.passwordHash == hashedPassword) {
            return {tokenMgr.issueToken(lifetime), result_type::OK};
        }
    }
    if (userCount == 0) {
        return {0, result_type::NO_ACCT};
    }
    return {0, result_type::BAD_PSWD};

}

bool AuthMgr::authenticateUser(token_type token) {
    return tokenMgr.containsToken(token);
}
