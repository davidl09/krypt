//
// Created by David Laeer on 2024-10-10.
//

#ifndef SETTINGS_H
#define SETTINGS_H

#include "defs.h"

const static fs::path
SERVER_ROOT = "./",
STATIC_ROOT = SERVER_ROOT / "static",
AUTHMGR_DB = SERVER_ROOT / "authmgr.db3",
AUTHMGR_SCHEMA = SERVER_ROOT / "authmgr-schema.sql",
USERMGR_DB = SERVER_ROOT / "usermgr.db3",
USER_MGR_SCHEMA = SERVER_ROOT / "usermgr-schema.sql";

using Clock = system_clock;

#endif //SETTINGS_H
