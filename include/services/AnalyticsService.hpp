#pragma once
#include <nlohmann/json.hpp>
#include "repositories/AnswerRepository.hpp"
#include "repositories/ProgressRepository.hpp"
#include "repositories/UserRepository.hpp"

namespace als {
class AnalyticsService {
public:
    AnalyticsService(UserRepository& users, AnswerRepository& answers, ProgressRepository& progress)
        : users_(users), answers_(answers), progress_(progress) {}

    nlohmann::json performance(int64_t userId);
    nlohmann::json weakTopics(int64_t userId);
    nlohmann::json studentsOverview();  // teacher view

private:
    UserRepository& users_;
    AnswerRepository& answers_;
    ProgressRepository& progress_;
};
}  // namespace als
