#include "services/CourseService.hpp"

#include "utils/AppError.hpp"
#include "utils/Logger.hpp"
#include "utils/QuestionImporter.hpp"

namespace als {
namespace {
constexpr size_t kMaxMaterialBytes = 200 * 1024;
constexpr size_t kMaxImportBytes = 300 * 1024;
}  // namespace

std::vector<Course> CourseService::list() { return courses_.listCourses(); }

Course CourseService::get(int64_t id) {
    auto c = courses_.findCourse(id);
    if (!c) throw AppError(404, "Course not found");
    return *c;
}

Course CourseService::requireOwned(int64_t courseId, int64_t teacherId) {
    Course c = get(courseId);
    if (c.teacherId != teacherId) throw AppError(403, "You can only change your own courses");
    return c;
}

Course CourseService::create(int64_t teacherId, const std::string& title, const std::string& description) {
    if (title.size() < 3 || title.size() > 120) throw AppError(400, "Course title must be 3-120 characters");
    if (description.size() > 1000) throw AppError(400, "Description is too long (max 1000 characters)");
    if (courses_.findCourseByTitle(title)) throw AppError(409, "A course with this title already exists");
    return get(courses_.insertCourse(title, description, teacherId));
}

std::vector<Material> CourseService::materials(int64_t courseId) {
    get(courseId);  // 404 if the course does not exist
    return courses_.listMaterials(courseId);
}

Material CourseService::addMaterial(int64_t teacherId, int64_t courseId, const std::string& title, const std::string& body) {
    requireOwned(courseId, teacherId);
    if (title.empty() || title.size() > 160) throw AppError(400, "Material title must be 1-160 characters");
    if (body.empty()) throw AppError(400, "Material text is empty");
    if (body.size() > kMaxMaterialBytes) throw AppError(413, "Material is too large (max 200 KB of text)");
    return *courses_.findMaterial(courses_.insertMaterial(courseId, title, body));
}

void CourseService::deleteMaterial(int64_t teacherId, int64_t materialId) {
    auto m = courses_.findMaterial(materialId);
    if (!m) throw AppError(404, "Material not found");
    requireOwned(m->courseId, teacherId);
    courses_.deleteMaterial(materialId);
}

int CourseService::importQuestions(int64_t teacherId, int64_t courseId, const std::string& text) {
    Course course = requireOwned(courseId, teacherId);
    if (text.size() > kMaxImportBytes) throw AppError(413, "File is too large (max 300 KB)");

    ImportResult parsed = parseQuestionText(text, course.title);
    if (!parsed.errors.empty()) {
        std::string msg = "Import failed, nothing was saved. ";
        for (size_t i = 0; i < parsed.errors.size() && i < 5; ++i) msg += parsed.errors[i] + "; ";
        if (parsed.errors.size() > 5) msg += "and " + std::to_string(parsed.errors.size() - 5) + " more problem(s).";
        throw AppError(400, msg);
    }
    for (const auto& q : parsed.questions) questions_.create(q);
    Logger::get()->info("Imported {} questions into course '{}'", parsed.questions.size(), course.title);
    return static_cast<int>(parsed.questions.size());
}

void CourseService::ensureCoursesForSubjects(int64_t teacherId) {
    for (const auto& subject : courses_.questionSubjects())
        if (!courses_.findCourseByTitle(subject)) courses_.insertCourse(subject, "", teacherId);
}

}  // namespace als
