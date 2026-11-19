#include "database/Database.hpp"

#include <filesystem>
#include <stdexcept>

namespace als {

// ---------- Statement ----------
Statement::Statement(sqlite3* db, const std::string& sql) : db_(db), stmt_(nullptr) {
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt_, nullptr) != SQLITE_OK)
        throw std::runtime_error(std::string("SQL prepare failed: ") + sqlite3_errmsg(db_));
}
Statement::~Statement() {
    if (stmt_) sqlite3_finalize(stmt_);
}
Statement::Statement(Statement&& other) noexcept : db_(other.db_), stmt_(other.stmt_) {
    other.stmt_ = nullptr;
}
Statement& Statement::bind(int i, int v) { sqlite3_bind_int(stmt_, i, v); return *this; }
Statement& Statement::bind(int i, int64_t v) { sqlite3_bind_int64(stmt_, i, v); return *this; }
Statement& Statement::bind(int i, double v) { sqlite3_bind_double(stmt_, i, v); return *this; }
Statement& Statement::bind(int i, const std::string& v) {
    sqlite3_bind_text(stmt_, i, v.c_str(), static_cast<int>(v.size()), SQLITE_TRANSIENT);
    return *this;
}
bool Statement::step() {
    int rc = sqlite3_step(stmt_);
    if (rc == SQLITE_ROW) return true;
    if (rc == SQLITE_DONE) return false;
    throw std::runtime_error(std::string("SQL step failed: ") + sqlite3_errmsg(db_));
}
void Statement::run() { step(); }
int64_t Statement::getInt(int c) const { return sqlite3_column_int64(stmt_, c); }
double Statement::getDouble(int c) const { return sqlite3_column_double(stmt_, c); }
std::string Statement::getText(int c) const {
    const unsigned char* t = sqlite3_column_text(stmt_, c);
    return t ? reinterpret_cast<const char*>(t) : "";
}

// ---------- Database ----------
Database::Database(const std::string& path) {
    if (path != ":memory:") {
        auto parent = std::filesystem::path(path).parent_path();
        if (!parent.empty()) std::filesystem::create_directories(parent);
    }
    if (sqlite3_open(path.c_str(), &db_) != SQLITE_OK) {
        std::string msg = db_ ? sqlite3_errmsg(db_) : "out of memory";
        sqlite3_close(db_);
        throw std::runtime_error("Cannot open database '" + path + "': " + msg);
    }
    sqlite3_busy_timeout(db_, 5000);
    exec("PRAGMA foreign_keys = ON;");  // SQLite ignores foreign keys unless asked
    exec("PRAGMA journal_mode = WAL;");
}
Database::~Database() {
    if (db_) sqlite3_close(db_);
}
void Database::exec(const std::string& sql) {
    char* err = nullptr;
    if (sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &err) != SQLITE_OK) {
        std::string msg = err ? err : "unknown error";
        sqlite3_free(err);
        throw std::runtime_error("SQL exec failed: " + msg);
    }
}
Statement Database::prepare(const std::string& sql) { return Statement(db_, sql); }
int64_t Database::lastInsertId() const { return sqlite3_last_insert_rowid(db_); }

// ---------- Transaction ----------
Transaction::Transaction(Database& db) : db_(db) { db_.exec("BEGIN;"); }
Transaction::~Transaction() {
    if (!done_) {
        try { db_.exec("ROLLBACK;"); } catch (...) {}
    }
}
void Transaction::commit() {
    db_.exec("COMMIT;");
    done_ = true;
}

}  // namespace als
