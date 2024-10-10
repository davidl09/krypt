//
// Created by David Laeer on 2024-10-10.
//

#ifndef PASSWORDHASH_H
#define PASSWORDHASH_H

#include <string>

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


#endif //PASSWORDHASH_H
