#include "api/AnalyticsRoutes.hpp"

#include "middleware/AuthMiddleware.hpp"
#include "middleware/ErrorMiddleware.hpp"

namespace als {

void registerAnalyticsRoutes(ApiApp& app, ServiceRegistry& s) {
    CROW_ROUTE(app, "/api/analytics/performance")([&s](const crow::request& req) {
        return guarded([&] {
            auto who = requireAuth(req, s.jwt, "student");
            return ResponseBuilder::ok(s.analytics.performance(who.userId));
        });
    });

    CROW_ROUTE(app, "/api/analytics/weak-topics")([&s](const crow::request& req) {
        return guarded([&] {
            auto who = requireAuth(req, s.jwt, "student");
            return ResponseBuilder::ok(s.analytics.weakTopics(who.userId));
        });
    });

    // Teacher dashboard: one row per student.
    CROW_ROUTE(app, "/api/analytics/students")([&s](const crow::request& req) {
        return guarded([&] {
            requireAuth(req, s.jwt, "teacher");
            return ResponseBuilder::ok(s.analytics.studentsOverview());
        });
    });
}

}  // namespace als
