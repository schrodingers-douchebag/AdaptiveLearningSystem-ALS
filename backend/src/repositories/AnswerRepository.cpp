#include "repositories/AnswerRepository.hpp"

namespace als {

int64_t AnswerRepository::insert(const Answer& a) {
    db_.prepare("INSERT INTO answer_history(user_id, question_id, selected_option_id, is_correct, time_taken) VALUES (?,?,?,?,?);")
        .bind(1, a.userId).bind(2, a.questionId).bind(3, a.selectedOptionId)
        .bind(4, a.isCorrect ? 1 : 0).bind(5, a.timeTaken).run();
    return db_.lastInsertId();
}

std::vector<AttemptRecord> AnswerRepository::recentAttempts(int64_t userId, int limit) {
    auto st = db_.prepare(
        "SELECT h.question_id, q.topic, q.difficulty, h.is_correct, h.time_taken "
        "FROM answer_history h JOIN questions q ON q.id = h.question_id "
        "WHERE h.user_id = ? ORDER BY h.id DESC LIMIT ?;");
    st.bind(1, userId).bind(2, limit);

    std::vector<AttemptRecord> out;
    while (st.step()) {
        AttemptRecord r;
        r.questionId = st.getInt(0);
        r.topic = st.getText(1);
        r.difficulty = static_cast<int>(st.getInt(2));
        r.correct = st.getInt(3) != 0;
        r.timeTaken = static_cast<int>(st.getInt(4));
        out.push_back(std::move(r));
    }
    return out;
}

std::unordered_map<int64_t, int> AnswerRepository::answeredCounts(int64_t userId) {
    auto st = db_.prepare("SELECT question_id, COUNT(*) FROM answer_history WHERE user_id = ? GROUP BY question_id;");
    st.bind(1, userId);
    std::unordered_map<int64_t, int> out;
    while (st.step()) out[st.getInt(0)] = static_cast<int>(st.getInt(1));
    return out;
}

int64_t AnswerRepository::totalCount(int64_t userId) {
    auto st = db_.prepare("SELECT COUNT(*) FROM answer_history WHERE user_id = ?;");
    st.bind(1, userId);
    st.step();
    return st.getInt(0);
}

}  // namespace als
