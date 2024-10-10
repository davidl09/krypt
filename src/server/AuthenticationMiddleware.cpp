//
// Created by David Laeer on 2024-10-06.
//

#include "AuthenticationMiddleware.h"

#include <server.h>

#include "defs.h"
using namespace std;

void AuthenticationMiddleware::before_handle(crow::request &req, crow::response &res, context &ctx) {
    const string credsEncoded = req.get_header_value("Authorization").substr(6);
    const string creds = crow::utility::base64decode(credsEncoded);

    const size_t found = creds.find(':');
    if (found > creds.size()) {
        res.code = crow::BAD_REQUEST;
        res.end("Bad Encoding");
    }

    const string
        username = creds.substr(0, found),
        password = creds.substr(found + 1);

    static Server* const server = Server::get_instance();

    if (not server->authenticateUser(username, password)) {
        res.code = crow::FORBIDDEN;
        res.end("Bad Credentials");
    }
}

void AuthenticationMiddleware::after_handle(crow::request &req, crow::response &res, context &ctx) {
}
