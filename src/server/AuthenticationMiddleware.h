//
// Created by David Laeer on 2024-10-06.
//

#ifndef AUTHENTICATIONMIDDLEWARE_H
#define AUTHENTICATIONMIDDLEWARE_H
#include "defs.h"
#include <set>
#include <crow/middleware.h>


struct AuthenticationMiddleware : public crow::ILocalMiddleware {
    AuthenticationMiddleware() = default;
    ~AuthenticationMiddleware() = default;

    struct context {

    };

    void before_handle(crow::request& req, crow::response& res, context& ctx);
    void after_handle(crow::request& req, crow::response& res, context& ctx);

private:
    //static std::set<std::pair<int, decltype(steady_clock::now())>> authenticatedUsers;

};



#endif //AUTHENTICATIONMIDDLEWARE_H
