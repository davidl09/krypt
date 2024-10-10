//
// Created by David Laeer on 2024-10-10.
//
#include "PasswordHash.h"
#include <openssl/evp.h>
#include <array>
#include <numeric>
#include <format>

PasswordHash::PasswordHash(const std::string & password)
    : passwordHash(hashStringToSha256Hex(password)){
}

PasswordHash::PasswordHash(std::string &&str)
    : passwordHash(hashStringToSha256Hex(str)) {}

PasswordHash::operator std::string() const {
    return passwordHash;
}


inline std::string PasswordHash::hashStringToSha256Hex(std::string_view input) {
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

