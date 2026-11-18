#include "repositories/ProgressRepository.hpp"

namespace als {
namespace {
LearningProgress readProgress(Statement& st) {
    LearningProgress p;
    p.id = st.getInt(0);
    p.userId = st.getInt(1);
    p.topic = st.getText(2);
    p.masteryScore = st.getDouble(3);
    p.totalAttempts = static_cast<int>(st.getInt(4));
    p.correctAttempts = static_cast<int>(st.getInt(5));
    p.updatedAt = st.getText(6);
    return p;
}
const char* kCols = "id, user_id, topic, mastery_score, total_attempts, correct_attempts, updated_at";
}  // namespace

std::optional<LearningProgress> ProgressRepository::get(int64_t userId, const std::string& topic) {
    auto st = db_.prepare(std::string("SELECT ") + kCols + " FROM learning_progress WHERE user_id = ? AND topic = ?;");
    st.bind(1, userId).bind(2, topic);
    if (!st.step()) return std::nullopt;
    return readProgress(st);
}

std::vector<LearningProgress> ProgressRepository::all(int64_t userId) {
    auto st = db_.prepare(std::string("SELECT ") + kCols + " FROM learning_progress WHERE user_id = ? ORDER BY topic;");
    st.bind(1, userId);
    std::vector<LearningProgress> out;
    while (st.step()) out.push_back(readProgress(st));
    return out;
}

void ProgressRepository::upsert(const LearningProgress& p) {
    db_.prepare(
           "INSERT INTO learning_progress(user_id, topic, mastery_score, total_attempts, correct_attempts) "
           "VALUES (?,?,?,?,?) "
           "ON CONFLICT(user_id, topic) DO UPDATE SET mastery_score = excluded.mastery_score, "
           "total_attempts = excluded.total_attempts, correct_attempts = excluded.correct_attempts, "
           "updated_at = datetime('now');")
        .bind(1, p.userId).bind(2, p.topic).bind(3, p.masteryScore)
        .bind(4, p.totalAttempts).bind(5, p.correctAttempts).run();
}

StudentProfile ProgressRepository::ensureProfile(int64_t userId, int startLevel) {
    db_.prepare("INSERT OR IGNORE INTO student_profiles(user_id, current_level) VALUES (?, ?);")
        .bind(1, userId).bind(2, startLevel).run();
    auto st = db_.prepare("SELECT id, user_id, current_level FROM student_profiles WHERE user_id = ?;");
    st.bind(1, userId);
    st.step();
    StudentProfile p;
    p.id = st.getInt(0);
    p.userId = st.getInt(1);
    p.currentLevel = static_cast<int>(st.getInt(2));
    return p;
}

void ProgressRepository::setLevel(int64_t userId, int level) {
    db_.prepare("UPDATE student_profiles SET current_level = ? WHERE user_id = ?;").bind(1, level).bind(2, userId).run();
}

}  // namespace als
