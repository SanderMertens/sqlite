-- https://www.sqlite.org/lang_createtable.html

CREATE TABLE IF NOT EXISTS "Objects" (
    "ObjectId" TEXT PRIMARY KEY,
    "Name" TEXT,
    "Parent" TEXT,
    "Type" TEXT,
    "State" INTEGER,
);
