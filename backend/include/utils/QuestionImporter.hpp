#pragma once
#include <string>
#include <vector>
#include "models/Question.hpp"

namespace als {

struct ImportResult {
    std::vector<Question> questions;
    std::vector<std::string> errors;  // "Block 3: ..." (empty = the whole text is valid)
};

// Turns a plain-text question bank into Question objects. Format (blocks separated by a blank line):
//
//   TOPIC: Arrays            (optional, default "General")
//   DIFFICULTY: 2            (optional, 1-5, default 2)
//   Q: What is the index of the first element of a C array?
//   A) 1
//   B) 0
//   C) -1
//   ANSWER: B
//
// Lines starting with '#' are comments. Every question gets `subject` as its subject.
// Pure function: no database, no HTTP (so it is unit-tested directly).
ImportResult parseQuestionText(const std::string& text, const std::string& subject);

}  // namespace als
