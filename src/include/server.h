#pragma once

#include "UserMgr.h"
#include "ChatMgr.h"
#include "crow/middlewares/session.h"
#include "crow.h"

class Server {
public:
    static Server *get_instance(std::string_view databaseFile, std::string_view configSQLFile);
    void run(int port);
    void run_async(int port);
    const static fs::path SERVER_ROOT, STATIC_ROOT;
private:
    Server(const fs::path &databaseFile, const fs::path& configSQLFile);

    using Session = crow::SessionMiddleware<crow::InMemoryStore>;
    crow::App<crow::CookieParser, Session> app;
    UserMgr db;
    //ChatMgr chatMgr;
    static Server *instance;


    constexpr static auto
        passwordKey = "password",
        emailKey = "email",
        tagKey = "tag",
        phoneNumberKey = "phone_number",
        nameKey = "name";
};
