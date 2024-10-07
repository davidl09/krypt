//
// Created by David Laeer on 2024-10-06.
//

#ifndef TOKENMGR_H
#define TOKENMGR_H


#include <algorithm>

#include "defs.h"

template <typename T = uint32_t>
class TokenMgr {
    enum class TokenState {
        VALID,
        EXPIRED,
    };
public:
    using timePoint = decltype(system_clock::now());
    TokenMgr() = default;
    ~TokenMgr() = default;

    bool containsToken(T token) {
        switch (getState(token)) {
            case TokenState::VALID:
                return true;
            case TokenState::EXPIRED: {
                std::scoped_lock lock{mapMutex};
                tokens.erase(token);
            }
        }
        return false;
    }

    void flushStaleTokens() {
        std::scoped_lock lock{mapMutex};

        for (auto it = tokens.begin(); it != tokens.end(); ++it) {
            if (getState(it->first) == TokenState::EXPIRED) {
                it = tokens.erase(it);
            }
        }

    }

    bool putToken(T token, auto lifeDuration = 1h) {
        std::scoped_lock lock{mapMutex};
        switch (getState(token)) {
            case TokenState::VALID:
                return false;

            case TokenState::EXPIRED:
                tokens[token] = system_clock::now() + lifeDuration;
        }

        return true;
    }

    bool removeToken(T token) {
        //returns true if token was present and has been deleted, else false
        std::scoped_lock lock{mapMutex};
        if (tokens.contains(token)) {
            tokens.erase(token);
            return true;
        }
        return false;
    }

    [[nodiscard]] size_t size() {

        {
            std::scoped_lock lock{mapMutex};
            std::erase_if(tokens, [this](auto token) {
                return isExpired(token.second);
            });
        }

        return tokens.size();
    }

private:

    TokenState getState(T token) const {
        if (not tokens.contains(token) || isExpired(tokens.at(token))) {
            return TokenState::EXPIRED;
        }
        return TokenState::VALID;
    }

    static bool isExpired(const timePoint expirationDate) {
        return expirationDate < system_clock::now();
    }
    std::mutex mapMutex;
    std::map<T, timePoint> tokens;
};



#endif //TOKENMGR_H
