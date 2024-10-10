//
// Created by David Laeer on 2024-10-06.
//

#ifndef TOKENMGR_H
#define TOKENMGR_H

#include "defs.h"
#include "settings.h"
#include <algorithm>
#include <mutex>
#include <unordered_map>

template <typename T = uint32_t>
class TokenMgr {
    enum class TokenState {
        EXPIRED,
        VALID
    };
public:
    using timePoint = decltype(Clock::now());
    TokenMgr() = default;
    ~TokenMgr() = default;

    T issueToken(Clock::duration lifetime) {
        while (containsToken(nextToken++)){}

        assert(putToken(nextToken, lifetime));
        return nextToken++;
    }

    bool containsToken(T token) {
        switch (getState(token)) {
            case TokenState::VALID:
                return true;
            case TokenState::EXPIRED: {
                std::scoped_lock lock{mapMutex};
                issuedTokens.erase(token);
            }
        }
        return false;
    }

    void flushStaleTokens() {
        std::scoped_lock lock{mapMutex};

        for (auto it = issuedTokens.begin(); it != issuedTokens.end(); ++it) {
            if (getState(it->first) == TokenState::EXPIRED) {
                it = issuedTokens.erase(it);
            }
        }

    }

    bool putToken(T token, auto lifeDuration = 1h) {
        std::scoped_lock lock{mapMutex};
        switch (getState(token)) {
            case TokenState::VALID:
                return false;

            case TokenState::EXPIRED:
                issuedTokens[token] = Clock::now() + lifeDuration;
        }

        return true;
    }

    bool removeToken(T token) {
        //returns true if token was present and has been deleted, else false
        std::scoped_lock lock{mapMutex};
        if (issuedTokens.contains(token)) {
            issuedTokens.erase(token);
            return true;
        }
        return false;
    }

    [[nodiscard]] size_t size() {

        {
            std::scoped_lock lock{mapMutex};
            std::erase_if(issuedTokens, [this](auto token) {
                return isExpired(token.second);
            });
        }

        return issuedTokens.size();
    }

private:

    TokenState getState(T token) const {
        if (not issuedTokens.contains(token) || isExpired(issuedTokens.at(token))) {
            return TokenState::EXPIRED;
        }
        return TokenState::VALID;
    }

    static bool isExpired(const timePoint expirationDate) {
        return expirationDate < Clock::now();
    }

    std::mutex mapMutex;
    std::unordered_map<T, timePoint> issuedTokens;
    T nextToken{0};
};



#endif //TOKENMGR_H
