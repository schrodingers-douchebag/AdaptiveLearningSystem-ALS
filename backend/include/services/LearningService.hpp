#pragma once
#include <optional>
#include "algorithms/AdaptiveEngine.hpp"
#include "database/Database.hpp"
#include "services/RecommendationService.hpp"

namespace als {
struct SubmitResult {
    bool isCorrect = false;
    std::string feedback;
    int64_t correctOptionId = 0;
    std::string topic;
    double updatedMastery = 0.0;
    int previousDifficulty = 2;
    int nextDifficulty = 2;
    std::string levelReason;
    std::optional<ScoredQuestion> next;
    std::optional<Question> nextQuestion;
    PerformanceSummary summary;
};

struct StartResult {
    int level = 2;
    std::optional<ScoredQuestion> pick;
    std::optional<Question> question;
};

class LearningService {
public:
    LearningService(Database& db, QuestionRepository& questions, AnswerRepository& answers,
                    ProgressRepository& progress, RecommendationRepository& recs,
                    RecommendationService& recommendation, const AdaptiveEngine& engine)
        : db_(db), questions_(questions), answers_(answers), progress_(progress), recs_(recs),
          recommendation_(recommendation), engine_(engine) {}

    StartResult start(int64_t userId);
    // The server decides correctness. The client only says which option it picked.
    SubmitResult submitAnswer(int64_t userId, int64_t questionId, int64_t optionId, int timeTaken);
    std::vector<LearningProgress> progress(int64_t userId);
    int level(int64_t userId);

private:
    Database& db_;
    QuestionRepository& questions_;
    AnswerRepository& answers_;
    ProgressRepository& progress_;
    RecommendationRepository& recs_;
    RecommendationService& recommendation_;
    const AdaptiveEngine& engine_;
};
}  // namespace als
