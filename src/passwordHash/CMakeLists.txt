add_library(
        passwordHash STATIC
        PasswordHash.cpp
)

target_include_directories(passwordHash PUBLIC ${INCLUDE_DIR})

target_link_libraries(
        passwordHash PRIVATE
        OpenSSL::SSL
        OpenSSL::Crypto
)