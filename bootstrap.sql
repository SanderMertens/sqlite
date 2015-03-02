-- https://www.sqlite.org/lang_createtable.html

CREATE TABLE IF NOT EXISTS Objects (
    Name TEXT,
    Parent NUMERIC,
    FOREIGN KEY(Parent) REFERENCES Objects(Oid)
);
