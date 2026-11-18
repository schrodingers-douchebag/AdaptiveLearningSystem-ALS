#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>
#include "models/Question.hpp"

namespace als {

struct RecommenderInput {
    int targetLevel = 2;
    std::unordered_map<std::string, double> topicMastery;      // topic -> 0..1 (missing = unknown)
    std::unordered_map<std::string, double> topicRecentError;  // topic -> recent error rate 0..1
    double overallRecentError = 0.0;
    std::unordered_map<int64_t, int> answeredCounts;           // question id -> times answered
    std::vector<int64_t> recentQuestionIds;                    // newest first
};

struct ScoredQuestion {
    int64_t questionId = 0;
    std::string topic;
    int difficulty = 1;
    double score = 0.0;
    // the four normalised factors (0..1)
    double weakTopic = 0.0;       // M
    double difficultyFit = 0.0;   // D
    double recentRelevance = 0.0; // R
    double novelty = 0.0;         // N
};

// S(q) = 0.35*M + 0.30*D + 0.20*R + 0.15*N
class QuestionRecommender {
public:
    // Returns up to `topK` questions, best first. Recently asked questions are skipped when possible.
    static std::vector<ScoredQuestion> recommend(const std::vector<Question>& candidates,
                                                 const RecommenderInput& in, size_t topK = 1);
    static ScoredQuestion score(const Question& q, const RecommenderInput& in);
};

}  // namespace als
