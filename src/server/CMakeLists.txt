add_library(
        server STATIC
        server.cpp
        AuthenticationMiddleware.cpp
        AuthMgr.cpp
)

target_include_directories(server PUBLIC ${INCLUDE_DIR})


target_link_libraries(
        server PUBLIC
        sqlpp11::sqlpp11
        database
        sqlite3
        OpenSSL::SSL 
        OpenSSL::Crypto
        Crow::Crow
)