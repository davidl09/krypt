#include "UserMgr.h"
#include "server.h"
#include "TokenMgr.h"

#include "crow.h"


int main()
{
    std::unique_ptr<Server> server(Server::get_instance());

    server->run(8080);
    return 0;

}
