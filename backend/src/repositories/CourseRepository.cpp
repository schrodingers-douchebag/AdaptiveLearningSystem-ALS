#include "repositories/CourseRepository.hpp"

namespace als {
namespace {
const char* kCourseSelect =
    "SELECT c.id, c.title, c.description, c.teacher_id, u.username, c.created_at, "
    "(SELECT COUNT(*) FROM materials m WHERE m.course_id = c.id), "
    "(SELECT COUNT(*) FROM questions q WHERE q.subject = c.title COLLATE NOCASE) "
    "FROM courses c JOIN users u ON u.id = c.teacher_id";

Course readCourse(Statement& st) {
    Course c;
    c.id = st.getInt(0);
    c.title = st.getText(1);
    c.description = st.getText(2);
    c.teacherId = st.getInt(3);
    c.teacherName = st.getText(4);
    c.createdAt = st.getText(5);
    c.materialCount = static_cast<int>(st.getInt(6));
    c.questionCount = static_cast<int>(st.getInt(7));
    return c;
}

Material readMaterial(Statement& st) {
    Material m;
    m.id = st.getInt(0);
    m.courseId = st.getInt(1);
    m.title = st.getText(2);
    m.body = st.getText(3);
    m.createdAt = st.getText(4);
    return m;
}
}  // namespace

int64_t CourseRepository::insertCourse(const std::string& title, const std::string& description, int64_t teacherId) {
    db_.prepare("INSERT INTO courses(title, description, teacher_id) VALUES (?,?,?);")
        .bind(1, title).bind(2, description).bind(3, teacherId).run();
    return db_.lastInsertId();
}

std::optional<Course> CourseRepository::findCourse(int64_t id) {
    auto st = db_.prepare(std::string(kCourseSelect) + " WHERE c.id = ?;");
    st.bind(1, id);
    if (!st.step()) return std::nullopt;
    return readCourse(st);
}

std::optional<Course> CourseRepository::findCourseByTitle(const std::string& title) {
    auto st = db_.prepare(std::string(kCourseSelect) + " WHERE c.title = ? COLLATE NOCASE;");
    st.bind(1, title);
    if (!st.step()) return std::nullopt;
    return readCourse(st);
}

std::vector<Course> CourseRepository::listCourses() {
    auto st = db_.prepare(std::string(kCourseSelect) + " ORDER BY c.id;");
    std::vector<Course> out;
    while (st.step()) out.push_back(readCourse(st));
    return out;
}

int64_t CourseRepository::insertMaterial(int64_t courseId, const std::string& title, const std::string& body) {
    db_.prepare("INSERT INTO materials(course_id, title, body) VALUES (?,?,?);")
        .bind(1, courseId).bind(2, title).bind(3, body).run();
    return db_.lastInsertId();
}

std::optional<Material> CourseRepository::findMaterial(int64_t id) {
    auto st = db_.prepare("SELECT id, course_id, title, body, created_at FROM materials WHERE id = ?;");
    st.bind(1, id);
    if (!st.step()) return std::nullopt;
    return readMaterial(st);
}

std::vector<Material> CourseRepository::listMaterials(int64_t courseId) {
    auto st = db_.prepare("SELECT id, course_id, title, body, created_at FROM materials WHERE course_id = ? ORDER BY id;");
    st.bind(1, courseId);
    std::vector<Material> out;
    while (st.step()) out.push_back(readMaterial(st));
    return out;
}

void CourseRepository::deleteMaterial(int64_t id) {
    db_.prepare("DELETE FROM materials WHERE id = ?;").bind(1, id).run();
}

std::vector<std::string> CourseRepository::questionSubjects() {
    auto st = db_.prepare("SELECT DISTINCT subject FROM questions ORDER BY subject;");
    std::vector<std::string> out;
    while (st.step()) out.push_back(st.getText(0));
    return out;
}

}  // namespace als
