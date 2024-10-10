//
// Created by David Laeer on 2024-10-06.
//

#ifndef AUTHENTICATIONMIDDLEWARE_H
#define AUTHENTICATIONMIDDLEWARE_H

#include "defs.h"
#include <crow/middleware.h>


struct AuthenticationMiddleware final : crow::ILocalMiddleware {
    AuthenticationMiddleware() = default;
    ~AuthenticationMiddleware() = default;

    struct context {};

    static void before_handle(crow::request& req, crow::response& res, context& ctx);

    static void after_handle(crow::request& req, crow::response& res, context& ctx);

private:

};



#endif //AUTHENTICATIONMIDDLEWARE_H
