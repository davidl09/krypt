#include <gtest/gtest.h>
#include "database.h"
#include "server.h"
#include "crow.h"

const auto databasePath = Server::SERVER_ROOT / "testdatabase.db3", configSQLPath = Server::SERVER_ROOT / "schema.sql";

TEST(Database, UserCount) {
    database db(databasePath, configSQLPath);

    db.clearAllUsers();
    ASSERT_TRUE(db.numUsers() == 0);

    constexpr std::string pswd = "12345678", email = "test@gmail.com";
    ASSERT_TRUE(
        db.addUser(
            email,
            pswd,
            "John Doe",
            "user123",
            "3435499889"
        )
    );
    ASSERT_TRUE(db.numUsers() == 1);
    db.clearAllUsers();
    ASSERT_TRUE(db.numUsers() == 0);

}

TEST(Database, AddUser) {
    database db(databasePath, configSQLPath);
    db.clearAllUsers();

    

    constexpr std::string pswd = "12345678", email = "test@gmail.com";
    ASSERT_TRUE(
        db.addUser(
            email,
            pswd,
            "John Doe",
            "user123",
            "3435499889"
        )
    );
    
    ASSERT_TRUE(db.validateUser(email, pswd));

    ASSERT_FALSE(
        db.addUser(
            email,
            pswd,
            "John Doe",
            "user123",
            "3435499889"
        )
    );

    db.deleteUser(
        email, 
        pswd
    );

    ASSERT_FALSE(
        db.validateUser(email, pswd)
    );

}



int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
