#pragma once
#include <nlohmann/json.hpp>
#include "models/Question.hpp"

namespace als {
// Students never see which option is correct (revealAnswer = false).
inline nlohmann::json toJson(const Question& q, bool revealAnswer = false) {
    nlohmann::json j = {{"id", q.id}, {"subject", q.subject}, {"topic", q.topic},
                        {"question_text", q.text}, {"difficulty", q.difficulty}};
    if (!q.options.empty()) {
        j["options"] = nlohmann::json::array();
        for (const auto& o : q.options) {
            nlohmann::json oj = {{"id", o.id}, {"text", o.text}};
            if (revealAnswer) oj["is_correct"] = o.isCorrect;
            j["options"].push_back(oj);
        }
    }
    if (revealAnswer) j["correct_answer"] = q.correctAnswer;
    return j;
}
}  // namespace als
