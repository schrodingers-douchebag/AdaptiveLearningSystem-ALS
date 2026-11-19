#include "api/RecommendationRoutes.hpp"

#include "dto/QuestionResponse.hpp"
#include "dto/RecommendationResponse.hpp"
#include "middleware/AuthMiddleware.hpp"
#include "middleware/ErrorMiddleware.hpp"

namespace als {

void registerRecommendationRoutes(ApiApp& app, ServiceRegistry& s) {
    CROW_ROUTE(app, "/api/recommendations/next")([&s](const crow::request& req) {
        return guarded([&] {
            auto who = requireAuth(req, s.jwt, "student");
            auto pick = s.recommendations.next(who.userId);
            if (!pick) return ResponseBuilder::ok({{"recommendation", nlohmann::json(nullptr)}, {"question", nlohmann::json(nullptr)}});
            return ResponseBuilder::ok({{"recommendation", toJson(*pick)},
                                        {"question", toJson(s.questions.get(pick->questionId))}});
        });
    });
}

}  // namespace als
