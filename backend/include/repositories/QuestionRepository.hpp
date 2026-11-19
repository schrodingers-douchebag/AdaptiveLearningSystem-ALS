#pragma once
#include <optional>
#include <string>
#include <vector>
#include "database/Database.hpp"
#include "models/Question.hpp"

namespace als {
struct QuestionFilter {
    std::string subject;  // empty = any
    std::string topic;    // empty = any
    int difficulty = 0;   // 0 = any
    int limit = 50;
    int offset = 0;
};

class QuestionRepository {
public:
    explicit QuestionRepository(Database& db) : db_(db) {}

    // Inserts the question and its options in one transaction. Exactly one option must be correct.
    int64_t insert(const Question& q);
    std::optional<Question> findById(int64_t id, bool withOptions = true);
    std::vector<Question> list(const QuestionFilter& filter);  // light mode: no options
    std::vector<Question> listAll();                           // for the recommender
    int64_t count();

private:
    Database& db_;
};
}  // namespace als
