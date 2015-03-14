-- https://www.sqlite.org/lang_createtable.html

CREATE TABLE IF NOT EXISTS "Objects" (
    "ObjectId" INTEGER PRIMARY KEY,
    "Name" TEXT,
    "Parent" NUMERIC REFERENCES "Objects"("ObjectId")
);
