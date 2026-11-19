#include "FileManager.h"
#include <filesystem>
#include <fstream>
#include <sstream>

void ensureDataFiles(const std::string& dir) {
    std::filesystem::create_directories(dir);
    const std::string qpath = dir + "/questions.txt";
    if (!std::filesystem::exists(qpath)) {
        std::ofstream out(qpath);
        out << "101|Arrays|1|15|5\n"
            << "102|Sorting|2|25|9\n"
            << "103|Graphs|3|40|12\n"
            << "104|Dynamic Programming|3|35|13\n"
            << "105|Strings|1|10|4\n"
            << "106|Greedy|2|20|8\n";
    }
    for (const std::string name : {"students.txt", "progress.txt"}) {
        const std::string path = dir + "/" + name;
        if (!std::filesystem::exists(path)) std::ofstream(path);
    }
}

std::vector<Question> loadQuestions(const std::string& path) {
    std::ifstream in(path);
    std::vector<Question> questions;
    std::string line;
    while (std::getline(in, line)) {
        std::stringstream ss(line);
        std::string id, topic, difficulty, minutes, score;
        if (!std::getline(ss, id, '|') || !std::getline(ss, topic, '|') ||
            !std::getline(ss, difficulty, '|') || !std::getline(ss, minutes, '|') ||
            !std::getline(ss, score, '|')) continue;
        try {
            questions.push_back({std::stoi(id), topic, std::stoi(difficulty),
                                  std::stoi(minutes), std::stoi(score)});
        } catch (...) { /* Ignore malformed rows. */ }
    }
    return questions;
}

void saveQuestions(const std::string& path, const std::vector<Question>& questions) {
    std::ofstream out(path);
    for (const auto& q : questions)
        out << q.id << '|' << q.topic << '|' << q.difficulty << '|'
            << q.estimatedMinutes << '|' << q.score << '\n';
}
