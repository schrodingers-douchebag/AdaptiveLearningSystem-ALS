#pragma once
#include <memory>
#include "algorithms/AdaptiveEngine.hpp"
#include "app_config.hpp"
#include "database/Database.hpp"
#include "middleware/CorsMiddleware.hpp"
#include "repositories/AnswerRepository.hpp"
#include "repositories/ProgressRepository.hpp"
#include "repositories/QuestionRepository.hpp"
#include "repositories/RecommendationRepository.hpp"
#include "repositories/UserRepository.hpp"
#include "services/AnalyticsService.hpp"
#include "services/AuthService.hpp"
#include "services/LearningService.hpp"
#include "services/QuestionService.hpp"
#include "services/RecommendationService.hpp"
#include "services/UserService.hpp"
#include "utils/JwtManager.hpp"

namespace als {

// Plain references handed to each route file.
struct ServiceRegistry {
    JwtManager& jwt;
    AuthService& auth;
    UserService& users;
    QuestionService& questions;
    LearningService& learning;
    RecommendationService& recommendations;
    AnalyticsService& analytics;
};

// Composition root: builds repositories -> services, then wires up the routes.
class ApiServer {
public:
    ApiServer(Database& db, const AppConfig& config);
    void prepareData();      // seed questions + demo accounts (only if empty / enabled)
    void start(int port);    // blocks until the server stops

private:
    const AppConfig& config_;
    JwtManager jwt_;
    AdaptiveEngine engine_;

    UserRepository userRepo_;
    QuestionRepository questionRepo_;
    AnswerRepository answerRepo_;
    ProgressRepository progressRepo_;
    RecommendationRepository recRepo_;

    AuthService auth_;
    UserService users_;
    QuestionService questions_;
    RecommendationService recommendations_;
    LearningService learning_;
    AnalyticsService analytics_;
};

}  // namespace als
