set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/src/include)

find_package(OpenSSL REQUIRED)

add_compile_options(-Wall)

add_executable(
        crypt_server
        main.cpp
        include/settings.h
        include/PasswordHash.h
)

target_include_directories(crypt_server PUBLIC include)

add_subdirectory(passwordHash)
add_subdirectory(database)
add_subdirectory(server)

target_link_libraries(
        crypt_server PRIVATE
        passwordHash
        database
        server
        Crow::Crow 
        sqlpp11::sqlpp11 
        sqlite3
        z
        OpenSSL::SSL 
        OpenSSL::Crypto
)
