set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/src/include)

find_package(OpenSSL REQUIRED)

add_executable(
        crypt_server
        main.cpp
)

target_include_directories(crypt_server PUBLIC include)

add_subdirectory(database)
add_subdirectory(chatmgr)
add_subdirectory(server)

target_link_libraries(
        crypt_server PRIVATE 
        database
        server
        Crow::Crow 
        sqlpp11::sqlpp11 
        sqlite3
        OpenSSL::SSL 
        OpenSSL::Crypto
)
