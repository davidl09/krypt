#include "database.h"
#include "server.h"

#include "crow.h"


int main()
{
    std::unique_ptr<Server> server(Server::getInstance(
    "database.db3",
    "schema.sql"
    ));

    server->run(8080);
    return 0;

}
