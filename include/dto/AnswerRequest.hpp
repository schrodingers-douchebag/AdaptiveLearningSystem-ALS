#pragma once
#include <cstdint>
#include <nlohmann/json.hpp>
#include "utils/AppError.hpp"

namespace als {
struct AnswerRequest {
    int64_t questionId = 0;
    int64_t selectedOptionId = 0;
    int timeTaken = 0;  // seconds

    static AnswerRequest fromJson(const nlohmann::json& j) {
        if (!j.contains("question_id") || !j.contains("selected_option_id"))
            throw AppError(400, "question_id and selected_option_id are required");
        AnswerRequest r;
        r.questionId = j.at("question_id").get<int64_t>();
        r.selectedOptionId = j.at("selected_option_id").get<int64_t>();
        r.timeTaken = j.value("time_taken", 0);
        return r;
    }
};
}  // namespace als
