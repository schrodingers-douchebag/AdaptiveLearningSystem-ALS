#include "api/ApiServer.hpp"

#include "api/AnalyticsRoutes.hpp"
#include "api/AuthRoutes.hpp"
#include "api/CourseRoutes.hpp"
#include "api/LearningRoutes.hpp"
#include "api/QuestionRoutes.hpp"
#include "api/RecommendationRoutes.hpp"
#include "utils/Logger.hpp"
#include "utils/ResponseBuilder.hpp"

namespace als {

ApiServer::ApiServer(Database& db, const AppConfig& config)
    : config_(config),
      jwt_(config.jwtSecret, config.jwtTtlMinutes),
      userRepo_(db), questionRepo_(db), answerRepo_(db), progressRepo_(db), recRepo_(db), courseRepo_(db),
      auth_(userRepo_, progressRepo_, jwt_),
      users_(userRepo_, progressRepo_, answerRepo_),
      questions_(questionRepo_),
      recommendations_(questionRepo_, answerRepo_, progressRepo_, recRepo_, engine_),
      learning_(db, questionRepo_, answerRepo_, progressRepo_, recRepo_, recommendations_, engine_),
      analytics_(userRepo_, answerRepo_, progressRepo_),
      courses_(courseRepo_, questions_) {}

void ApiServer::prepareData() {
    questions_.seedFromFile(config_.seedFile);
    if (config_.seedDemoUsers) {
        auth_.ensureDemoAccounts();
        // Give the seeded questions a course each (owned by the demo teacher).
        if (auto teacher = userRepo_.findByUsernameOrEmail("teacher")) courses_.ensureCoursesForSubjects(teacher->id);
    }
}

void ApiServer::start(int port) {
    ApiApp app;
    app.loglevel(crow::LogLevel::Warning);
    configureCors(app, config_.corsOrigin);

    ServiceRegistry services{jwt_, auth_, users_, questions_, learning_, recommendations_, analytics_, courses_};

    CROW_ROUTE(app, "/api/health")([] {
        return ResponseBuilder::ok({{"status", "ok"}, {"application", "Adaptive Learning System"}});
    });
    registerAuthRoutes(app, services);
    registerQuestionRoutes(app, services);
    registerLearningRoutes(app, services);
    registerRecommendationRoutes(app, services);
    registerAnalyticsRoutes(app, services);
    registerCourseRoutes(app, services);

    Logger::get()->info("API listening on http://localhost:{}", port);
    // Single worker thread on purpose: all services share one SQLite connection.
    app.port(static_cast<uint16_t>(port)).run();
}

}  // namespace als
