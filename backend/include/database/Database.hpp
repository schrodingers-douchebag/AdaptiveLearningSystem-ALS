#pragma once
#include <sqlite3.h>
#include <cstdint>
#include <string>

namespace als {

// A prepared SQL statement (RAII: finalised automatically).
class Statement {
public:
    Statement(sqlite3* db, const std::string& sql);
    ~Statement();
    Statement(const Statement&) = delete;
    Statement& operator=(const Statement&) = delete;
    Statement(Statement&& other) noexcept;
    Statement& operator=(Statement&&) = delete;

    // Parameters are 1-based, like in SQLite.
    Statement& bind(int index, int value);
    Statement& bind(int index, int64_t value);
    Statement& bind(int index, double value);
    Statement& bind(int index, const std::string& value);

    bool step();  // true = a row is ready, false = finished
    void run();   // execute a statement that returns no rows

    int64_t getInt(int col) const;
    double getDouble(int col) const;
    std::string getText(int col) const;

private:
    sqlite3* db_;
    sqlite3_stmt* stmt_;
};

class Database {
public:
    explicit Database(const std::string& path);  // ":memory:" is allowed (used by tests)
    ~Database();
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;

    void exec(const std::string& sql);
    Statement prepare(const std::string& sql);
    int64_t lastInsertId() const;

private:
    sqlite3* db_ = nullptr;
};

// BEGIN ... COMMIT. If commit() is never called, the destructor rolls back.
class Transaction {
public:
    explicit Transaction(Database& db);
    ~Transaction();
    Transaction(const Transaction&) = delete;
    Transaction& operator=(const Transaction&) = delete;
    void commit();

private:
    Database& db_;
    bool done_ = false;
};

}  // namespace als
