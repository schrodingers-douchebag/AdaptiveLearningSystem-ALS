#pragma once
#include <optional>
#include "algorithms/AdaptiveEngine.hpp"
#include "repositories/AnswerRepository.hpp"
#include "repositories/ProgressRepository.hpp"
#include "repositories/QuestionRepository.hpp"
#include "repositories/RecommendationRepository.hpp"

namespace als {
class RecommendationService {
public:
    RecommendationService(QuestionRepository& questions, AnswerRepository& answers, ProgressRepository& progress,
                          RecommendationRepository& recs, const AdaptiveEngine& engine)
        : questions_(questions), answers_(answers), progress_(progress), recs_(recs), engine_(engine) {}

    // Loads the student's level, topic mastery and history from the database.
    StudentState buildState(int64_t userId);
    std::optional<ScoredQuestion> next(int64_t userId, bool persist = true);

private:
    QuestionRepository& questions_;
    AnswerRepository& answers_;
    ProgressRepository& progress_;
    RecommendationRepository& recs_;
    const AdaptiveEngine& engine_;
};
}  // namespace als
