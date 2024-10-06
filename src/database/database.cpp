//
// Created by David Laeer on 2024-08-26.
//


#include "database.h"
#include "defs.h"
#include "ddl.h"

#include "sqlpp11/sqlpp11.h"
#include "sqlpp11/sqlite3/sqlite3.h"


#include <openssl/sha.h>

#include <fstream>
#include <string>
#include <numeric>
#include <openssl/evp.h>

using namespace sqlpp;
using namespace db;

uint64_t database::numUsers() {
    Accounts user;
    uint64_t count = 0;

    for (const auto& u : db(select(user.tag).from(user).unconditionally())) {
        ++count;
    }

    return count;
}

database::database(const fs::path& dbPath, const fs::path& configPath = "")
    : config(dbPath, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE), db(config) {

    if (std::ifstream cfgCmd{configPath}; cfgCmd) {
        std::string command((std::istreambuf_iterator(cfgCmd)),
                             std::istreambuf_iterator<char>());
        db.execute(command);
    } else {
        throw std::runtime_error("Failed to open config file");
    }
}

bool database::addUser(
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

bool database::deleteUser(const std::string &email, const PasswordHash &password) {
    Accounts user;
    if (not validateUser(email, password)) {}
    db(remove_from(user)
        .where(
            user.email.like(email)
            and user.passwordHash.like(static_cast<std::string>(password))
        )
    );
    return true;
}

bool database::validateUser(const std::string& email, const PasswordHash &password) {
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

Accounts database::getProfile(const std::string &email, const PasswordHash &password) {
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
void database::clearAllUsers() {
    Accounts user;
    db(remove_from(user).unconditionally());
}

database::PasswordHash::PasswordHash(const std::string & password)
    : passwordHash(hashStringToSha256Hex(password)){
}

database::PasswordHash::PasswordHash(std::string &&str)
    : passwordHash(hashStringToSha256Hex(str)) {}

database::PasswordHash::operator std::string() const {
    return passwordHash;
}

// database::PasswordHash::operator std::string() && {
//     return std::move(passwordHash);
// }

std::string database::PasswordHash::hashStringToSha256Hex(std::string_view input) {
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
#endif

