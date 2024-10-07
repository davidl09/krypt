//
// Created by David Laeer on 2024-10-05.
//

#ifndef CHATMGR_H
#define CHATMGR_H

#include <database.h>

#include "defs.h"

class ChatMgr final : public Database {
public:
    ChatMgr(const fs::path& dbPath, const fs::path& configPath);
    ~ChatMgr();


private:
};

#endif //CHATMGR_H
