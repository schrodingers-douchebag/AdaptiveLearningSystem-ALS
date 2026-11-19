#include "api/LearningRoutes.hpp"

#include "dto/AnswerRequest.hpp"
#include "dto/ProgressResponse.hpp"
#include "dto/QuestionResponse.hpp"
#include "dto/RecommendationResponse.hpp"
#include "middleware/AuthMiddleware.hpp"
#include "middleware/ErrorMiddleware.hpp"

namespace als {

void registerLearningRoutes(ApiApp& app, ServiceRegistry& s) {
    CROW_ROUTE(app, "/api/learning/start").methods(crow::HTTPMethod::Post)([&s](const crow::request& req) {
        return guarded([&] {
            auto who = requireAuth(req, s.jwt, "student");
            auto r = s.learning.start(who.userId);
            nlohmann::json j = {{"level", r.level}};
            j["question"] = r.question ? toJson(*r.question) : nlohmann::json(nullptr);
            j["recommendation"] = r.pick ? toJson(*r.pick) : nlohmann::json(nullptr);
            return ResponseBuilder::ok(j);
        });
    });

    CROW_ROUTE(app, "/api/learning/submit-answer").methods(crow::HTTPMethod::Post)([&s](const crow::request& req) {
        return guarded([&] {
            auto who = requireAuth(req, s.jwt, "student");
            auto a = AnswerRequest::fromJson(ResponseBuilder::parseBody(req));
            auto r = s.learning.submitAnswer(who.userId, a.questionId, a.selectedOptionId, a.timeTaken);

            nlohmann::json j = {
                {"is_correct", r.isCorrect}, {"feedback", r.feedback}, {"correct_option_id", r.correctOptionId},
                {"topic", r.topic}, {"updated_mastery", r.updatedMastery},
                {"previous_difficulty", r.previousDifficulty}, {"next_difficulty", r.nextDifficulty},
                {"difficulty_reason", r.levelReason}, {"performance", toJson(r.summary)}};
            j["next_recommendation"] = r.next ? nlohmann::json(r.next->questionId) : nlohmann::json(nullptr);
            j["next_question"] = r.nextQuestion ? toJson(*r.nextQuestion) : nlohmann::json(nullptr);
            return ResponseBuilder::ok(j);
        });
    });

    CROW_ROUTE(app, "/api/learning/progress")([&s](const crow::request& req) {
        return guarded([&] {
            auto who = requireAuth(req, s.jwt, "student");
            nlohmann::json topics = nlohmann::json::array();
            for (const auto& p : s.learning.progress(who.userId)) topics.push_back(toJson(p));
            return ResponseBuilder::ok({{"level", s.learning.level(who.userId)}, {"topics", topics}});
        });
    });
}

}  // namespace als
