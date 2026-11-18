#pragma once
#include <cstdlib>
#include <fstream>
#include <stdexcept>
#include <string>
#include <nlohmann/json.hpp>

namespace als {

// Settings read from config/config.json. Env vars JWT_SECRET and PORT win over the file.
struct AppConfig {
    int port = 8080;
    std::string dbPath = "data/adaptive_learning.db";
    std::string seedFile = "data/seed_questions.json";
    std::string logFile = "logs/app.log";
    std::string corsOrigin = "http://localhost:5173";
    std::string jwtSecret = "CHANGE-ME-set-JWT_SECRET-env-var-in-real-use";
    int jwtTtlMinutes = 120;
    bool seedDemoUsers = true;

    bool usesDefaultSecret() const { return jwtSecret.rfind("CHANGE-ME", 0) == 0; }

    static AppConfig load(const std::string& path) {
        AppConfig c;
        std::ifstream in(path);
        if (in) {
            nlohmann::json j = nlohmann::json::parse(in, nullptr, false);
            if (j.is_discarded()) throw std::runtime_error("config file is not valid JSON: " + path);
            c.port = j.value("port", c.port);
            c.dbPath = j.value("db_path", c.dbPath);
            c.seedFile = j.value("seed_file", c.seedFile);
            c.logFile = j.value("log_file", c.logFile);
            c.corsOrigin = j.value("cors_origin", c.corsOrigin);
            c.jwtSecret = j.value("jwt_secret", c.jwtSecret);
            c.jwtTtlMinutes = j.value("jwt_ttl_minutes", c.jwtTtlMinutes);
            c.seedDemoUsers = j.value("seed_demo_users", c.seedDemoUsers);
        }
        if (const char* s = std::getenv("JWT_SECRET")) c.jwtSecret = s;
        if (const char* p = std::getenv("PORT")) c.port = std::atoi(p);
        return c;
    }
};

}  // namespace als
