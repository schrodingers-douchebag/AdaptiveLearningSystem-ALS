#pragma once
#include <optional>
#include <string>
#include <vector>
#include "repositories/QuestionRepository.hpp"

namespace als {
class QuestionService {
public:
    explicit QuestionService(QuestionRepository& questions) : questions_(questions) {}

    std::vector<Question> list(const QuestionFilter& filter);
    Question get(int64_t id);
    Question create(Question q);  // validates, then stores (teacher only at the API level)
    // Loads data/seed_questions.json when the table is empty. Returns how many were added.
    int seedFromFile(const std::string& path);

private:
    QuestionRepository& questions_;
};
}  // namespace als
