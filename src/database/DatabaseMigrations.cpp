#include "database/DatabaseMigrations.hpp"

#include <string>
#include <vector>

namespace als {
namespace {

struct Migration {
    int version;
    const char* sql;
};

// Version 1 = the full first schema (same as scripts/setup_database.sql).
const std::vector<Migration> kMigrations = {
    {1, R"SQL(

CREATE TABLE IF NOT EXISTS users (
  id            INTEGER PRIMARY KEY AUTOINCREMENT,
  username      TEXT NOT NULL UNIQUE COLLATE NOCASE,
  email         TEXT NOT NULL UNIQUE COLLATE NOCASE,
  password_hash TEXT NOT NULL,
  role          TEXT NOT NULL DEFAULT 'student' CHECK (role IN ('student','teacher')),
  created_at    TEXT NOT NULL DEFAULT (datetime('now'))
);

CREATE TABLE IF NOT EXISTS questions (
  id             INTEGER PRIMARY KEY AUTOINCREMENT,
  subject        TEXT NOT NULL,
  topic          TEXT NOT NULL,
  question_text  TEXT NOT NULL,
  difficulty     INTEGER NOT NULL CHECK (difficulty BETWEEN 1 AND 5),
  correct_answer TEXT NOT NULL,
  created_at     TEXT NOT NULL DEFAULT (datetime('now'))
);
CREATE INDEX IF NOT EXISTS idx_questions_topic ON questions(topic, difficulty);

CREATE TABLE IF NOT EXISTS answer_options (
  id          INTEGER PRIMARY KEY AUTOINCREMENT,
  question_id INTEGER NOT NULL REFERENCES questions(id) ON DELETE CASCADE,
  option_text TEXT NOT NULL,
  is_correct  INTEGER NOT NULL DEFAULT 0 CHECK (is_correct IN (0,1))
);
CREATE INDEX IF NOT EXISTS idx_options_question ON answer_options(question_id);

CREATE TABLE IF NOT EXISTS student_profiles (
  id            INTEGER PRIMARY KEY AUTOINCREMENT,
  user_id       INTEGER NOT NULL UNIQUE REFERENCES users(id) ON DELETE CASCADE,
  current_level INTEGER NOT NULL DEFAULT 2 CHECK (current_level BETWEEN 1 AND 5)
);

CREATE TABLE IF NOT EXISTS answer_history (
  id                 INTEGER PRIMARY KEY AUTOINCREMENT,
  user_id            INTEGER NOT NULL REFERENCES users(id) ON DELETE CASCADE,
  question_id        INTEGER NOT NULL REFERENCES questions(id) ON DELETE CASCADE,
  selected_option_id INTEGER NOT NULL REFERENCES answer_options(id),
  is_correct         INTEGER NOT NULL CHECK (is_correct IN (0,1)),
  time_taken         INTEGER NOT NULL DEFAULT 0,
  created_at         TEXT NOT NULL DEFAULT (datetime('now'))
);
CREATE INDEX IF NOT EXISTS idx_history_user ON answer_history(user_id, id DESC);

CREATE TABLE IF NOT EXISTS learning_progress (
  id               INTEGER PRIMARY KEY AUTOINCREMENT,
  user_id          INTEGER NOT NULL REFERENCES users(id) ON DELETE CASCADE,
  topic            TEXT NOT NULL,
  mastery_score    REAL NOT NULL DEFAULT 0.5 CHECK (mastery_score BETWEEN 0 AND 1),
  total_attempts   INTEGER NOT NULL DEFAULT 0,
  correct_attempts INTEGER NOT NULL DEFAULT 0,
  updated_at       TEXT NOT NULL DEFAULT (datetime('now')),
  UNIQUE (user_id, topic)
);

CREATE TABLE IF NOT EXISTS recommendations (
  id          INTEGER PRIMARY KEY AUTOINCREMENT,
  user_id     INTEGER NOT NULL REFERENCES users(id) ON DELETE CASCADE,
  question_id INTEGER NOT NULL REFERENCES questions(id) ON DELETE CASCADE,
  score       REAL NOT NULL,
  created_at  TEXT NOT NULL DEFAULT (datetime('now'))
);
CREATE INDEX IF NOT EXISTS idx_recs_user ON recommendations(user_id, id DESC);
)SQL"},
    // Add future changes here, e.g. {2, "ALTER TABLE questions ADD COLUMN explanation TEXT;"},
};

}  // namespace

void DatabaseMigrations::apply(Database& db) {
    db.exec("CREATE TABLE IF NOT EXISTS schema_version (version INTEGER NOT NULL);");

    int current = 0;
    {
        auto st = db.prepare("SELECT COALESCE(MAX(version), 0) FROM schema_version;");
        if (st.step()) current = static_cast<int>(st.getInt(0));
    }

    for (const auto& m : kMigrations) {
        if (m.version <= current) continue;
        Transaction tx(db);
        db.exec(m.sql);
        db.prepare("INSERT INTO schema_version(version) VALUES (?);").bind(1, m.version).run();
        tx.commit();
    }
}

}  // namespace als
