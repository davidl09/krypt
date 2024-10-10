add_library(
        database STATIC
        database.cpp
        userdatabase.cpp
)

target_include_directories(database PUBLIC ${INCLUDE_DIR})


target_link_libraries(
        database PRIVATE
        passwordHash
        sqlpp11::sqlpp11
        sqlite3
        OpenSSL::SSL
        OpenSSL::Crypto
)