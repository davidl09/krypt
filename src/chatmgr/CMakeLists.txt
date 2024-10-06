add_library(chatMgr STATIC
        ChatMgr.cpp
)

target_include_directories(chatMgr PUBLIC ${INCLUDE_DIR})

