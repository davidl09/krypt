add_library(
        server STATIC
        server.cpp
)

target_include_directories(server PUBLIC ${INCLUDE_DIR})

target_compile_options(server PRIVATE -Wall)

target_link_libraries(
        server PUBLIC
        sqlpp11::sqlpp11
        sqlite3
        OpenSSL::SSL 
        OpenSSL::Crypto
        Crow::Crow
)