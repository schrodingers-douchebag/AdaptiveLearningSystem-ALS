#pragma once
#include <crow.h>
#include <crow/middlewares/cors.h>
#include <string>

namespace als {
using ApiApp = crow::App<crow::CORSHandler>;

// Lets the React dev server (another origin) call this API from the browser.
inline void configureCors(ApiApp& app, const std::string& origin) {
    auto& cors = app.get_middleware<crow::CORSHandler>();
    cors.global()
        .origin(origin)
        .methods(crow::HTTPMethod::Get, crow::HTTPMethod::Post, crow::HTTPMethod::Put,
                 crow::HTTPMethod::Delete, crow::HTTPMethod::Options)
        .headers("Content-Type", "Authorization")
        .max_age(600);
}
}  // namespace als
