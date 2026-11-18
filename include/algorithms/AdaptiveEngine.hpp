#pragma once
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>
#include "algorithms/KnowledgeTracker.hpp"
#include "algorithms/PerformanceAnalyzer.hpp"
#include "algorithms/QuestionRecommender.hpp"
#include "models/Answer.hpp"
#include "models/Question.hpp"

namespace als {

// Everything the engine needs to know about a student. Built by the service layer from the database;
// the engine itself never touches SQLite or HTTP.
struct StudentState {
    int level = 2;
    std::unordered_map<std::string, TopicMastery> topics;
    std::vector<AttemptRecord> recentAttempts;  // newest first
    std::unordered_map<int64_t, int> answeredCounts;
};

struct AnswerEvent {
    int64_t questionId = 0;
    std::string topic;
    int difficulty = 1;
    bool correct = false;
    int timeTaken = 0;
};

struct EngineResult {
    std::string topic;
    TopicMastery updatedTopic;
    int previousLevel = 2;
    int newLevel = 2;
    std::string levelReason;
    PerformanceSummary summary;
    std::optional<ScoredQuestion> next;
};

class AdaptiveEngine {
public:
    // 1 record answer -> 2 update topic mastery -> 3 analyse recent performance
    // -> 4 adjust difficulty -> 5 rank questions -> 6 return the best next one
    EngineResult processAnswer(StudentState state, const AnswerEvent& event,
                               const std::vector<Question>& candidates) const;

    std::optional<ScoredQuestion> recommendNext(const StudentState& state,
                                                const std::vector<Question>& candidates) const;

private:
    static RecommenderInput buildInput(const StudentState& state, int level);
};

}  // namespace als
