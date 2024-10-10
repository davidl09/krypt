CREATE TABLE IF NOT EXISTS credentials (
                                        email TEXT UNIQUE NOT NULL,
                                        password_hash TEXT NOT NULL
);