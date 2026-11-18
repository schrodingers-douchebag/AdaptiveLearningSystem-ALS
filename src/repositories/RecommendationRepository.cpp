#include "repositories/RecommendationRepository.hpp"

namespace als {

void RecommendationRepository::save(int64_t userId, int64_t questionId, double score) {
    db_.prepare("INSERT INTO recommendations(user_id, question_id, score) VALUES (?,?,?);")
        .bind(1, userId).bind(2, questionId).bind(3, score).run();
}

std::vector<Recommendation> RecommendationRepository::recent(int64_t userId, int limit) {
    auto st = db_.prepare("SELECT id, user_id, question_id, score, created_at FROM recommendations WHERE user_id = ? ORDER BY id DESC LIMIT ?;");
    st.bind(1, userId).bind(2, limit);
    std::vector<Recommendation> out;
    while (st.step()) {
        Recommendation r;
        r.id = st.getInt(0);
        r.userId = st.getInt(1);
        r.questionId = st.getInt(2);
        r.score = st.getDouble(3);
        r.createdAt = st.getText(4);
        out.push_back(r);
    }
    return out;
}

}  // namespace als
