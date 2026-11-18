#include <iostream>

#include "api/ApiServer.hpp"
#include "app_config.hpp"
#include "database/Database.hpp"
#include "database/DatabaseInitializer.hpp"
#include "utils/Logger.hpp"

int main() {
    try {
        // Run this program from the project root so "config/..." and "data/..." resolve.
        als::AppConfig config = als::AppConfig::load("config/config.json");
        als::Logger::init(config.logFile);
        als::Logger::get()->info("Starting Adaptive Learning System...");
        if (config.usesDefaultSecret())
            als::Logger::get()->warn("Using the default JWT secret. Set the JWT_SECRET environment variable before deploying.");

        als::Database database(config.dbPath);
        als::DatabaseInitializer(database).initialize();

        als::ApiServer server(database, config);
        server.prepareData();
        server.start(config.port);
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Startup error: " << e.what() << '\n';
        return 1;
    }
}
