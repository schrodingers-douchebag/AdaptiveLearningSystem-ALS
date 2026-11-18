#pragma once
#include <nlohmann/json.hpp>
#include "repositories/AnswerRepository.hpp"
#include "repositories/ProgressRepository.hpp"
#include "repositories/UserRepository.hpp"

namespace als {
class UserService {
public:
    UserService(UserRepository& users, ProgressRepository& progress, AnswerRepository& answers)
        : users_(users), progress_(progress), answers_(answers) {}
    nlohmann::json profile(int64_t userId);

private:
    UserRepository& users_;
    ProgressRepository& progress_;
    AnswerRepository& answers_;
};
}  // namespace als
