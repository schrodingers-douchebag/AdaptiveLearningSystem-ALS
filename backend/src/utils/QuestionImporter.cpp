#include "utils/QuestionImporter.hpp"

#include <cctype>
#include <cstdlib>
#include <sstream>

namespace als {
namespace {

constexpr size_t kMaxQuestions = 500;

std::string trim(const std::string& s) {
    size_t a = 0, b = s.size();
    while (a < b && std::isspace(static_cast<unsigned char>(s[a]))) ++a;
    while (b > a && std::isspace(static_cast<unsigned char>(s[b - 1]))) --b;
    return s.substr(a, b - a);
}

std::string lower(std::string s) {
    for (auto& c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return s;
}

struct Block {
    int number = 0;
    std::vector<std::string> lines;
};

// Parses one block; appends either a question or error messages.
void parseBlock(const Block& block, const std::string& subject, ImportResult& out) {
    const std::string where = "Block " + std::to_string(block.number) + ": ";
    std::string topic = "General", text, answer;
    int difficulty = 2;
    std::vector<std::string> options;
    std::vector<std::string> problems;

    for (const std::string& line : block.lines) {
        // Option line: "A) text" or "A. text" (letters A-F, in order).
        if (line.size() >= 3 && (line[1] == ')' || line[1] == '.') &&
            std::toupper(static_cast<unsigned char>(line[0])) >= 'A' &&
            std::toupper(static_cast<unsigned char>(line[0])) <= 'F') {
            const char letter = static_cast<char>(std::toupper(static_cast<unsigned char>(line[0])));
            const char expected = static_cast<char>('A' + options.size());
            if (letter != expected) {
                problems.push_back(where + "options must be in order A, B, C... (found " + std::string(1, letter) + ")");
                continue;
            }
            const std::string value = trim(line.substr(2));
            if (value.empty()) problems.push_back(where + "option " + std::string(1, letter) + " is empty");
            else options.push_back(value);
            continue;
        }

        const size_t colon = line.find(':');
        if (colon == std::string::npos) {
            problems.push_back(where + "unrecognised line: \"" + line.substr(0, 40) + "\"");
            continue;
        }
        const std::string key = lower(trim(line.substr(0, colon)));
        const std::string value = trim(line.substr(colon + 1));

        if (key == "q" || key == "question") {
            if (!text.empty()) problems.push_back(where + "more than one Q: line");
            text = value;
        } else if (key == "topic") {
            if (!value.empty()) topic = value;
        } else if (key == "difficulty") {
            char* end = nullptr;
            const long d = std::strtol(value.c_str(), &end, 10);
            if (value.empty() || *end != '\0' || d < 1 || d > 5) problems.push_back(where + "DIFFICULTY must be 1-5");
            else difficulty = static_cast<int>(d);
        } else if (key == "answer") {
            answer = value;
        } else {
            problems.push_back(where + "unknown field \"" + key + "\"");
        }
    }

    if (text.empty()) problems.push_back(where + "missing Q: line");
    if (options.size() < 2) problems.push_back(where + "needs at least 2 options (A), B), ...)");
    if (options.size() > 6) problems.push_back(where + "at most 6 options are allowed");

    int correct = -1;
    if (answer.size() == 1 && std::toupper(static_cast<unsigned char>(answer[0])) >= 'A') {
        correct = std::toupper(static_cast<unsigned char>(answer[0])) - 'A';
    }
    if (answer.empty()) problems.push_back(where + "missing ANSWER: line");
    else if (correct < 0 || correct >= static_cast<int>(options.size()))
        problems.push_back(where + "ANSWER must be one of the option letters (A-" +
                           std::string(1, static_cast<char>('A' + (options.empty() ? 0 : options.size() - 1))) + ")");

    if (!problems.empty()) {
        out.errors.insert(out.errors.end(), problems.begin(), problems.end());
        return;
    }

    Question q;
    q.subject = subject;
    q.topic = topic;
    q.text = text;
    q.difficulty = difficulty;
    for (size_t i = 0; i < options.size(); ++i) q.options.push_back({0, options[i], static_cast<int>(i) == correct});
    out.questions.push_back(std::move(q));
}

}  // namespace

ImportResult parseQuestionText(const std::string& text, const std::string& subject) {
    ImportResult out;
    std::istringstream in(text);
    std::string raw;
    Block current;
    int blockCount = 0;
    std::vector<Block> blocks;

    auto flush = [&] {
        if (!current.lines.empty()) {
            current.number = ++blockCount;
            blocks.push_back(current);
        }
        current = Block{};
    };

    while (std::getline(in, raw)) {
        const std::string line = trim(raw);  // trim() also drops a trailing '\r' (Windows files)
        if (!line.empty() && line[0] == '#') continue;
        if (line.empty()) flush();
        else current.lines.push_back(line);
    }
    flush();

    if (blocks.empty()) out.errors.push_back("The file contains no questions");
    if (blocks.size() > kMaxQuestions)
        out.errors.push_back("Too many questions in one file (maximum " + std::to_string(kMaxQuestions) + ")");
    if (!out.errors.empty()) return out;

    for (const auto& b : blocks) parseBlock(b, subject, out);
    return out;
}

}  // namespace als
