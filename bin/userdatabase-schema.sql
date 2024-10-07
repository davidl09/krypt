CREATE TABLE IF NOT EXISTS accounts (
                                        id INTEGER PRIMARY KEY AUTOINCREMENT,
                                        email TEXT UNIQUE NOT NULL,
                                        password_hash TEXT NOT NULL,
                                        name TEXT NOT NULL,
                                        tag TEXT UNIQUE NOT NULL,
                                        time_created INTEGER NOT NULL,
                                        phone_number TEXT UNIQUE NOT NULL
);