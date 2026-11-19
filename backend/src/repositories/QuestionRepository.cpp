#include "repositories/QuestionRepository.hpp"

#include <stdexcept>

namespace als {
namespace {
const char* kCols = "id, subject, topic, question_text, difficulty, correct_answer";

Question readQuestion(Statement& st) {
    Question q;
    q.id = st.getInt(0);
    q.subject = st.getText(1);
    q.topic = st.getText(2);
    q.text = st.getText(3);
    q.difficulty = static_cast<int>(st.getInt(4));
    q.correctAnswer = st.getText(5);
    return q;
}
}  // namespace

int64_t QuestionRepository::insert(const Question& q) {
    int correctCount = 0;
    std::string correctText;
    for (const auto& o : q.options)
        if (o.isCorrect) { ++correctCount; correctText = o.text; }
    if (q.options.size() < 2 || correctCount != 1)
        throw std::invalid_argument("a question needs 2+ options and exactly one correct option");

    Transaction tx(db_);
    db_.prepare("INSERT INTO questions(subject, topic, question_text, difficulty, correct_answer) VALUES (?,?,?,?,?);")
        .bind(1, q.subject).bind(2, q.topic).bind(3, q.text).bind(4, q.difficulty).bind(5, correctText).run();
    const int64_t id = db_.lastInsertId();
    for (const auto& o : q.options)
        db_.prepare("INSERT INTO answer_options(question_id, option_text, is_correct) VALUES (?,?,?);")
            .bind(1, id).bind(2, o.text).bind(3, o.isCorrect ? 1 : 0).run();
    tx.commit();
    return id;
}

std::optional<Question> QuestionRepository::findById(int64_t id, bool withOptions) {
    auto st = db_.prepare(std::string("SELECT ") + kCols + " FROM questions WHERE id = ?;");
    st.bind(1, id);
    if (!st.step()) return std::nullopt;
    Question q = readQuestion(st);

    if (withOptions) {
        auto os = db_.prepare("SELECT id, option_text, is_correct FROM answer_options WHERE question_id = ? ORDER BY id;");
        os.bind(1, id);
        while (os.step()) q.options.push_back({os.getInt(0), os.getText(1), os.getInt(2) != 0});
    }
    return q;
}

std::vector<Question> QuestionRepository::list(const QuestionFilter& f) {
    std::string sql = std::string("SELECT ") + kCols + " FROM questions WHERE 1=1";
    if (!f.subject.empty()) sql += " AND subject = ? COLLATE NOCASE";
    if (!f.topic.empty()) sql += " AND topic = ? COLLATE NOCASE";
    if (f.difficulty > 0) sql += " AND difficulty = ?";
    sql += " ORDER BY id LIMIT ? OFFSET ?;";

    auto st = db_.prepare(sql);
    int i = 1;
    if (!f.subject.empty()) st.bind(i++, f.subject);
    if (!f.topic.empty()) st.bind(i++, f.topic);
    if (f.difficulty > 0) st.bind(i++, f.difficulty);
    st.bind(i++, f.limit).bind(i, f.offset);

    std::vector<Question> out;
    while (st.step()) out.push_back(readQuestion(st));
    return out;
}

std::vector<Question> QuestionRepository::listAll() {
    QuestionFilter f;
    f.limit = 100000;
    return list(f);
}

int64_t QuestionRepository::count() {
    auto st = db_.prepare("SELECT COUNT(*) FROM questions;");
    st.step();
    return st.getInt(0);
}

}  // namespace als
