#pragma once
#include <crow.h>
#include <string>
#include <nlohmann/json.hpp>

namespace als {
// Every reply has the same shape:  {"success": true, ...}  or  {"success": false, "error": "..."}
class ResponseBuilder {
public:
    static crow::response ok(nlohmann::json body = nlohmann::json::object(), int code = 200);
    static crow::response error(int code, const std::string& message);
    // Parses the request body as a JSON object or throws AppError(400).
    static nlohmann::json parseBody(const crow::request& req);
};
}  // namespace als
