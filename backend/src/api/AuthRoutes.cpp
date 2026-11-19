#include "api/AuthRoutes.hpp"

#include "dto/LoginRequest.hpp"
#include "dto/RegisterRequest.hpp"
#include "dto/UserResponse.hpp"
#include "middleware/AuthMiddleware.hpp"
#include "middleware/ErrorMiddleware.hpp"

namespace als {

void registerAuthRoutes(ApiApp& app, ServiceRegistry& s) {
    CROW_ROUTE(app, "/api/auth/register").methods(crow::HTTPMethod::Post)([&s](const crow::request& req) {
        return guarded([&] {
            auto r = RegisterRequest::fromJson(ResponseBuilder::parseBody(req));
            auto result = s.auth.registerStudent(r.username, r.email, r.password);
            return ResponseBuilder::ok({{"token", result.token}, {"user", toJson(result.user)}}, 201);
        });
    });

    CROW_ROUTE(app, "/api/auth/login").methods(crow::HTTPMethod::Post)([&s](const crow::request& req) {
        return guarded([&] {
            auto r = LoginRequest::fromJson(ResponseBuilder::parseBody(req));
            auto result = s.auth.login(r.identifier, r.password);
            return ResponseBuilder::ok({{"token", result.token}, {"user", toJson(result.user)}});
        });
    });

    CROW_ROUTE(app, "/api/users/profile")([&s](const crow::request& req) {
        return guarded([&] {
            auto who = requireAuth(req, s.jwt);
            return ResponseBuilder::ok(s.users.profile(who.userId));
        });
    });
}

}  // namespace als
