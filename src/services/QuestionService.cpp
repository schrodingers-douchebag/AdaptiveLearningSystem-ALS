#include "services/QuestionService.hpp"

#include <fstream>
#include <nlohmann/json.hpp>
#include "algorithms/AlgorithmConfig.hpp"
#include "utils/AppError.hpp"
#include "utils/Logger.hpp"

namespace als {

std::vector<Question> QuestionService::list(const QuestionFilter& f) { return questions_.list(f); }

Question QuestionService::get(int64_t id) {
    auto q = questions_.findById(id);
    if (!q) throw AppError(404, "Question not found");
    return *q;
}

Question QuestionService::create(Question q) {
    if (q.subject.empty() || q.topic.empty() || q.text.empty()) throw AppError(400, "subject, topic and question_text are required");
    if (q.difficulty < cfg::kMinLevel || q.difficulty > cfg::kMaxLevel) throw AppError(400, "difficulty must be 1-5");
    int correct = 0;
    for (const auto& o : q.options) correct += o.isCorrect ? 1 : 0;
    if (q.options.size() < 2 || q.options.size() > 6) throw AppError(400, "a question needs 2-6 options");
    if (correct != 1) throw AppError(400, "exactly one option must be correct");
    return get(questions_.insert(q));
}

int QuestionService::seedFromFile(const std::string& path) {
    if (questions_.count() > 0) return 0;
    std::ifstream in(path);
    if (!in) {
        Logger::get()->warn("Seed file not found: {} (starting with an empty question bank)", path);
        return 0;
    }
    auto j = nlohmann::json::parse(in, nullptr, false);
    if (!j.is_array()) throw std::runtime_error("seed file must be a JSON array: " + path);

    int added = 0;
    for (const auto& item : j) {
        Question q;
        q.subject = item.value("subject", "");
        q.topic = item.value("topic", "");
        q.text = item.value("text", "");
        q.difficulty = item.value("difficulty", 1);
        const int answer = item.value("answer", -1);  // index into "options"
        int i = 0;
        for (const auto& o : item.at("options")) q.options.push_back({0, o.get<std::string>(), i++ == answer});
        create(q);
        ++added;
    }
    Logger::get()->info("Seeded {} questions", added);
    return added;
}

}  // namespace als
