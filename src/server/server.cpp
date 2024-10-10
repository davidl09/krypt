#include "server.h"
#include "AuthenticationMiddleware.h"
#include <nlohmann/json.hpp>


using namespace nlohmann;

#include "settings.h"

Server *Server::instance = nullptr;

const fs::path Server::SERVER_ROOT = "./";

Server * Server::get_instance() {
    if (instance !=  nullptr) {
        return instance;
    }
    instance = new Server(
        USERMGR_DB, USER_MGR_SCHEMA
        );
    return instance;
}

void Server::run(const int port) {
    app.port(port).multithreaded().run();
}

void Server::run_async(const int port) {
    app.port(port).multithreaded().run_async();
}


Server::Server(const fs::path &databaseFile, const fs::path &configSQLFile)
    : db(databaseFile, configSQLFile) {

    CROW_ROUTE(app, "/createUser").methods("POST"_method)(
        [this](const crow::request& req){
            const auto body = json::parse(req.body);

            if (
                not(
                    body.contains(emailKey)
                and body.contains(tagKey)
                and body.contains(phoneNumberKey)
                and body.contains(nameKey)
                )
                    ) {
                return crow::response(crow::BAD_REQUEST, "Missing credentials in JSON body");
            }

            if (not db.addUser(
                body.at(emailKey),
                body.at(nameKey),
                body.at(tagKey),
                body.at(phoneNumberKey)
            )) {

                return crow::response(crow::BAD_REQUEST, "Could not create user");
            }
            return crow::response(crow::OK, "OK");
        }
    );

    CROW_ROUTE(app, "/deleteUser").methods("POST"_method)(
        [this](const crow::request& req) {
            try {
                const auto body = json::parse(req.body);

                if (
                    not (body.contains(emailKey)
                    and body.contains(passwordKey))) {
                    return crow::response(crow::BAD_REQUEST, "Missing credentials in JSON body");
                }

                if (not db.deleteUser(body.at(emailKey))) {
                    return crow::response(crow::BAD_REQUEST, "Could not delete user");
                }

                return crow::response(crow::OK, "OK");
            }
            catch (const json::exception &e) {
                return crow::response(crow::BAD_REQUEST, e.what());
            }
            catch (const std::exception &e) {
                return crow::response(crow::INTERNAL_SERVER_ERROR, e.what());
            }

        });

    // CROW_ROUTE(app, "/updateUser")
    // .methods("POST"_method)
    // .CROW_MIDDLEWARES(app, AuthenticationMiddleware)
    // ([this](const crow::request& req, crow::response& res) {
    //     const auto body = json::parse(req.body);
    //
    //
    //     if (body.contains(phoneNumberKey)) {
    //         const std::string newPhoneNumber = body.at(phoneNumberKey);
    //         if (not db.changePhonenumber()
    //     }
    //     //
    //     res.code = 200;
    //     res.end();
    // });

#ifndef NDEBUG
    CROW_ROUTE(app, "/testAuthMiddleware")
    .methods("POST"_method)
    //.CROW_MIDDLEWARES(app, AuthenticationMiddleware)
    (
        [](const crow::request& req, crow::response& res) {
            res.code = 200;
            res.end("OK");
        });
#endif

}
