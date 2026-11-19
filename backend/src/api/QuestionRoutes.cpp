#include "api/QuestionRoutes.hpp"

#include <algorithm>
#include <cstdlib>
#include "dto/QuestionResponse.hpp"
#include "middleware/AuthMiddleware.hpp"
#include "middleware/ErrorMiddleware.hpp"

namespace als {
namespace {
int intParam(const crow::request& req, const char* key, int fallback) {
    const char* v = req.url_params.get(key);
    return v ? std::atoi(v) : fallback;
}
std::string strParam(const crow::request& req, const char* key) {
    const char* v = req.url_params.get(key);
    return v ? v : "";
}
}  // namespace

void registerQuestionRoutes(ApiApp& app, ServiceRegistry& s) {
    // GET /api/questions?topic=&subject=&difficulty=&limit=&offset=
    CROW_ROUTE(app, "/api/questions").methods(crow::HTTPMethod::Get)([&s](const crow::request& req) {
        return guarded([&] {
            auto who = requireAuth(req, s.jwt);
            QuestionFilter f;
            f.subject = strParam(req, "subject");
            f.topic = strParam(req, "topic");
            f.difficulty = intParam(req, "difficulty", 0);
            f.limit = std::clamp(intParam(req, "limit", 50), 1, 200);
            f.offset = std::max(0, intParam(req, "offset", 0));

            const bool teacher = who.role == "teacher";
            nlohmann::json items = nlohmann::json::array();
            for (const auto& q : s.questions.list(f)) items.push_back(toJson(q, teacher));
            return ResponseBuilder::ok({{"count", items.size()}, {"questions", items}});
        });
    });

    CROW_ROUTE(app, "/api/questions/<int>").methods(crow::HTTPMethod::Get)([&s](const crow::request& req, int id) {
        return guarded([&] {
            auto who = requireAuth(req, s.jwt);
            return ResponseBuilder::ok({{"question", toJson(s.questions.get(id), who.role == "teacher")}});
        });
    });

    // Teachers can add questions: {subject, topic, question_text, difficulty, options:[{text,is_correct}]}
    CROW_ROUTE(app, "/api/questions").methods(crow::HTTPMethod::Post)([&s](const crow::request& req) {
        return guarded([&] {
            requireAuth(req, s.jwt, "teacher");
            auto j = ResponseBuilder::parseBody(req);
            Question q;
            q.subject = j.value("subject", "");
            q.topic = j.value("topic", "");
            q.text = j.value("question_text", "");
            q.difficulty = j.value("difficulty", 0);
            for (const auto& o : j.value("options", nlohmann::json::array()))
                q.options.push_back({0, o.value("text", ""), o.value("is_correct", false)});
            return ResponseBuilder::ok({{"question", toJson(s.questions.create(q), true)}}, 201);
        });
    });
}

}  // namespace als
