#pragma once
#include <string>
#include <vector>
#include "repositories/CourseRepository.hpp"
#include "services/QuestionService.hpp"

namespace als {
class CourseService {
public:
    CourseService(CourseRepository& courses, QuestionService& questions) : courses_(courses), questions_(questions) {}

    std::vector<Course> list();
    Course get(int64_t id);
    Course create(int64_t teacherId, const std::string& title, const std::string& description);
    std::vector<Material> materials(int64_t courseId);
    Material addMaterial(int64_t teacherId, int64_t courseId, const std::string& title, const std::string& body);
    void deleteMaterial(int64_t teacherId, int64_t materialId);
    // Parses the text (see utils/QuestionImporter.hpp). Saves nothing if any block is invalid.
    int importQuestions(int64_t teacherId, int64_t courseId, const std::string& text);
    // Creates a course for every question subject that has none (used for the seeded questions).
    void ensureCoursesForSubjects(int64_t teacherId);

private:
    Course requireOwned(int64_t courseId, int64_t teacherId);

    CourseRepository& courses_;
    QuestionService& questions_;
};
}  // namespace als
