#pragma once
#include "database.h"
#include "crow.h"

class Server {
public:
    static Server *getInstance(std::string_view databaseFile, std::string_view configSQLFile);
    void run(int port);
    void run_async(int port);
    const static fs::path SERVER_ROOT, STATIC_ROOT;
private:
    Server(const fs::path &databaseFile, const fs::path& configSQLFile);
    crow::SimpleApp app;
    database db;
    static Server *instance;


    constexpr static auto
        passwordKey = "password",
        emailKey = "email",
        tagKey = "tag",
        phoneNumberKey = "phone_number",
        nameKey = "name";
};
