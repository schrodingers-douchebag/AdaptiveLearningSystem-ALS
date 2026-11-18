#include "utils/ResponseBuilder.hpp"

#include "utils/AppError.hpp"

namespace als {

static crow::response make(int code, const nlohmann::json& body) {
    crow::response r(code, body.dump());
    r.add_header("Content-Type", "application/json");
    return r;
}

crow::response ResponseBuilder::ok(nlohmann::json body, int code) {
    if (!body.is_object()) body = {{"data", body}};
    body["success"] = true;
    return make(code, body);
}

crow::response ResponseBuilder::error(int code, const std::string& message) {
    return make(code, {{"success", false}, {"error", message}});
}

nlohmann::json ResponseBuilder::parseBody(const crow::request& req) {
    auto j = nlohmann::json::parse(req.body, nullptr, false);
    if (j.is_discarded() || !j.is_object()) throw AppError(400, "Request body must be a JSON object");
    return j;
}

}  // namespace als
