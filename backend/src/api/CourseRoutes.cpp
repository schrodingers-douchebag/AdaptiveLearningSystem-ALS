#include "api/CourseRoutes.hpp"

#include "dto/CourseResponse.hpp"
#include "middleware/AuthMiddleware.hpp"
#include "middleware/ErrorMiddleware.hpp"

namespace als {

void registerCourseRoutes(ApiApp& app, ServiceRegistry& s) {
    // GET /api/courses  -> every course with its material and question counts
    CROW_ROUTE(app, "/api/courses").methods(crow::HTTPMethod::Get)([&s](const crow::request& req) {
        return guarded([&] {
            requireAuth(req, s.jwt);
            nlohmann::json items = nlohmann::json::array();
            for (const auto& c : s.courses.list()) items.push_back(toJson(c));
            return ResponseBuilder::ok({{"courses", items}});
        });
    });

    // POST /api/courses  {title, description}   (teacher)
    CROW_ROUTE(app, "/api/courses").methods(crow::HTTPMethod::Post)([&s](const crow::request& req) {
        return guarded([&] {
            auto who = requireAuth(req, s.jwt, "teacher");
            auto j = ResponseBuilder::parseBody(req);
            auto c = s.courses.create(who.userId, j.value("title", ""), j.value("description", ""));
            return ResponseBuilder::ok({{"course", toJson(c)}}, 201);
        });
    });

    // GET /api/courses/:id -> the course and all its materials (with text)
    CROW_ROUTE(app, "/api/courses/<int>").methods(crow::HTTPMethod::Get)([&s](const crow::request& req, int id) {
        return guarded([&] {
            requireAuth(req, s.jwt);
            nlohmann::json mats = nlohmann::json::array();
            for (const auto& m : s.courses.materials(id)) mats.push_back(toJson(m));
            return ResponseBuilder::ok({{"course", toJson(s.courses.get(id))}, {"materials", mats}});
        });
    });

    // POST /api/courses/:id/materials  {title, body}   (the course's teacher)
    CROW_ROUTE(app, "/api/courses/<int>/materials").methods(crow::HTTPMethod::Post)([&s](const crow::request& req, int id) {
        return guarded([&] {
            auto who = requireAuth(req, s.jwt, "teacher");
            auto j = ResponseBuilder::parseBody(req);
            auto m = s.courses.addMaterial(who.userId, id, j.value("title", ""), j.value("body", ""));
            return ResponseBuilder::ok({{"material", toJson(m)}}, 201);
        });
    });

    // POST /api/courses/:id/questions/import  {text}   (the course's teacher)
    CROW_ROUTE(app, "/api/courses/<int>/questions/import").methods(crow::HTTPMethod::Post)([&s](const crow::request& req, int id) {
        return guarded([&] {
            auto who = requireAuth(req, s.jwt, "teacher");
            auto j = ResponseBuilder::parseBody(req);
            int n = s.courses.importQuestions(who.userId, id, j.value("text", ""));
            return ResponseBuilder::ok({{"imported", n}}, 201);
        });
    });

    // DELETE /api/materials/:id   (the course's teacher)
    CROW_ROUTE(app, "/api/materials/<int>").methods(crow::HTTPMethod::Delete)([&s](const crow::request& req, int id) {
        return guarded([&] {
            auto who = requireAuth(req, s.jwt, "teacher");
            s.courses.deleteMaterial(who.userId, id);
            return ResponseBuilder::ok();
        });
    });
}

}  // namespace als
