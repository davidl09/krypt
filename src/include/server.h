#pragma once

#include <AuthenticationMiddleware.h>
#include <AuthMgr.h>
#include <TokenMgr.h>

#include "UserMgr.h"
#include "crow/middlewares/session.h"
#include "crow/middlewares/cookie_parser.h"
#include "crow.h"

inline auto datetime_as_string() -> std::string {
    const auto now = Clock::now();
    return std::format("{:%d-%m-%Y_%H:%M:%OS}", now);
}

class Server {
public:
    static Server *get_instance();
    void run(int port);
    void run_async(int port);
    const static fs::path SERVER_ROOT, STATIC_ROOT;
    bool authenticateUser(const std::string& username, const std::string& password);

#ifndef NDEBUG
    auto& getApp() { return app; }
    auto& getUserDb() { return db; }
#endif

private:
    Server(const fs::path &databaseFile, const fs::path& configSQLFile);

    crow::App<crow::CookieParser> app;
    UserMgr db;
    AuthMgr authMgr;
    //ChatMgr chatMgr;
    static Server *instance;


    constexpr static auto
        passwordKey = "password",
        emailKey = "email",
        tagKey = "tag",
        phoneNumberKey = "phone_number",
        nameKey = "name";
};
