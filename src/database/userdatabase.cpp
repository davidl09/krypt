//
// Created by David Laeer on 2024-10-06.
//

#include "UserMgr.h"
#include <openssl/evp.h>
#include <numeric>



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
    const PasswordHash& password,
    const std::string& name,
    const std::string& tag,
    const std::string& phoneNumber) {
    try {
        Accounts user;
        const auto now = system_clock::now();
        const auto timestamp = std::chrono::duration_cast<seconds>(now.time_since_epoch()).count();

        std::scoped_lock lock(dbMutex);
        db(insert_into(user).set(
            user.email = email,
            user.passwordHash = static_cast<std::string>(password),
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

bool UserMgr::deleteUser(const std::string &email, const PasswordHash &password) {
    Accounts user;
    if (not validateUser(email, password)) {}
    db(remove_from(user)
        .where(
            user.email == email
            and user.passwordHash.like(static_cast<std::string>(password))
        )
    );
    return true;
}

bool UserMgr::validateUser(const std::string& email, const PasswordHash &password) {
    Accounts user;
    std::scoped_lock lock(dbMutex);
    const auto users =
    db(
        select(
            user.email, user.passwordHash
        ).from(user).where(
            user.passwordHash.like(std::string(std::move(password))) and user.email.like(email)
        )
    );
    return not users.empty();
}

Accounts UserMgr::getProfile(const std::string &email, const PasswordHash &password) {
    if (not validateUser(email, password)) {
        throw InvalidCredentialsException("Invalid email");
    }

    Accounts user;
    std::scoped_lock lock(dbMutex);

    db(select(user.id, user.email, user.tag, user.name, user.phoneNumber, user.timeCreated).from(user).where(
        user.email.like(email)
        ));

    return user;
}

#ifndef NDEBUG
void UserMgr::clearAllUsers() {
    Accounts user;
    db(remove_from(user).unconditionally());
}
#endif



UserMgr::PasswordHash::PasswordHash(const std::string & password)
    : passwordHash(hashStringToSha256Hex(password)){
}

UserMgr::PasswordHash::PasswordHash(std::string &&str)
    : passwordHash(hashStringToSha256Hex(str)) {}

UserMgr::PasswordHash::operator std::string() const {
    return passwordHash;
}


std::string UserMgr::PasswordHash::hashStringToSha256Hex(std::string_view input) {
    std::array<unsigned char, EVP_MAX_MD_SIZE> hash{};
    unsigned int hashLen;

    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr);
    EVP_DigestUpdate(ctx, input.data(), input.size());
    EVP_DigestFinal_ex(ctx, hash.data(), &hashLen);
    EVP_MD_CTX_free(ctx);

    // Convert the hash to a hexadecimal string (unchanged)
    return std::accumulate(hash.begin(), hash.begin() + hashLen, std::string{},
        [](auto acc, auto h) { return acc + std::format("{:02x}", h); });
}



