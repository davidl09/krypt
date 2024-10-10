//
// Created by David Laeer on 2024-10-06.
//

#include "UserMgr.h"
#include <openssl/evp.h>
#include "settings.h"


using namespace sqlpp;
using namespace db;

UserMgr::UserMgr(const fs::path &dbPath, const fs::path &configPath)
    : Database(dbPath, configPath) {}

uint64_t UserMgr::numUsers() {
    Accounts user;
    uint64_t count = 0;

    //improve this somehow?? check sqlpp11 api
    auto result = db(select(user.tag).from(user).unconditionally());
    for (auto it = result.begin(); it != result.end(); ++it) {
        ++count;
    }

    return count;
}

bool UserMgr::addUser(
    const std::string& email,
    const std::string& name,
    const std::string& tag,
    const std::string& phoneNumber) {
    try {
        Accounts user;
        const auto now = Clock::now();
        const auto timestamp = std::chrono::duration_cast<seconds>(now.time_since_epoch()).count();

        std::scoped_lock lock(dbMutex);
        db(insert_into(user).set(
            user.email = email,
            user.name = name,
            user.tag = tag,
            user.timeCreated = timestamp,
            user.phoneNumber = phoneNumber
        ));
        return true;
    } catch (const std::exception& e) {
#ifndef NDEBUG
        std::cerr << "Error adding user: " << e.what() << std::endl;
#endif
        return false;
    }
}

bool UserMgr::deleteUser(const std::string &email) {
    Accounts user;
    db(remove_from(user)
        .where(
            user.email == email
        )
    );
    return true;
}

Accounts UserMgr::getProfile(const std::string &email) {

    Accounts user;
    std::scoped_lock lock(dbMutex);

    db(select(user.id, user.email, user.tag, user.name, user.phoneNumber, user.timeCreated).from(user).where(
        user.email.like(email)
        ));

    return user;
}

bool UserMgr::changeEmail(const std::string &email, const std::string &newEmail) {

    Accounts user;
    std::scoped_lock lock(dbMutex);
    const auto result = db(update(user).set(user.email = newEmail).where(user.email.like(email)));
    return result;
}

bool UserMgr::changePhonenumber(const std::string &email, const std::string &newPhoneNumber) {

    Accounts user;
    const auto users =
        db(
            select(
                user.phoneNumber
                ).from(user)
                .where(user.email == email));

    //phone number already in use
    if (not users.empty()) {
        return false;
    }

    {
        std::scoped_lock lock(dbMutex);
        const auto result = db(update(user).set(user.phoneNumber = newPhoneNumber).where(user.email.like(email)));
        return result; //
    }

}

#ifndef NDEBUG
void UserMgr::clearAllUsers() {
    Accounts user;
    db(remove_from(user).unconditionally());
}
#endif


