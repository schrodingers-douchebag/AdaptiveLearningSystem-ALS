#pragma once
#include <nlohmann/json.hpp>
#include "algorithms/QuestionRecommender.hpp"

namespace als {
// Explains WHY a question was picked (handy for the algorithm demo screen).
inline nlohmann::json toJson(const ScoredQuestion& s) {
    return {{"question_id", s.questionId}, {"topic", s.topic}, {"difficulty", s.difficulty},
            {"score", s.score},
            {"factors", {{"weak_topic", s.weakTopic}, {"difficulty_fit", s.difficultyFit},
                         {"recent_relevance", s.recentRelevance}, {"novelty", s.novelty}}}};
}
}  // namespace als
