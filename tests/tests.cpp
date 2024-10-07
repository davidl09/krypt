#include <gtest/gtest.h>
#include "TokenMgr.h"
#include "UserMgr.h"
#include "server.h"
#include "crow.h"

const auto databasePath = Server::SERVER_ROOT / "testdatabase.db3", configSQLPath = Server::SERVER_ROOT / "userdatabase-schema.sql";

TEST(Database, UserCount) {
    UserMgr db(databasePath, configSQLPath);

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
    UserMgr db(databasePath, configSQLPath);
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

TEST(TokenMgr, deletion) {
    TokenMgr tokenMgr;
    for (int i = 0; i < 100; ++i) {
        EXPECT_TRUE(tokenMgr.putToken(i, 1s));
    }

    EXPECT_EQ(tokenMgr.size(), 100);

    std::this_thread::sleep_for(1.01s);
    EXPECT_EQ(tokenMgr.size(), 0);
}

TEST(TokenMgr, deletion2) {
    TokenMgr tokenMgr;
    constexpr auto NUM_TOKENS = 1000;

    for (int i = 0; i < NUM_TOKENS / 2; ++i) {
        tokenMgr.putToken(i, 1s);
    }

    for (int i = 0; i < NUM_TOKENS / 2; ++i) {
        tokenMgr.putToken(i + NUM_TOKENS, 2s);
    }

    EXPECT_EQ(tokenMgr.size(), NUM_TOKENS);
    std::this_thread::sleep_for(1.01s);
    EXPECT_EQ(tokenMgr.size(), NUM_TOKENS / 2);
    std::this_thread::sleep_for(1.01s);
    EXPECT_EQ(tokenMgr.size(), 0);
}

TEST(TokenMgr, insertion) {
    TokenMgr tokenMgr;
    for (int i = 0; i < 100; ++i) {
        tokenMgr.putToken(i, 1s);
    }
    for (int i = 0; i < 100; ++i) {
        tokenMgr.putToken(i, 1s);
    }
    EXPECT_EQ(tokenMgr.size(), 100);
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
