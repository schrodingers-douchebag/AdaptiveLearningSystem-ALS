#pragma once
#include <optional>
#include <string>
#include <vector>
#include "database/Database.hpp"
#include "models/Course.hpp"

namespace als {
class CourseRepository {
public:
    explicit CourseRepository(Database& db) : db_(db) {}

    int64_t insertCourse(const std::string& title, const std::string& description, int64_t teacherId);
    std::optional<Course> findCourse(int64_t id);
    std::optional<Course> findCourseByTitle(const std::string& title);
    std::vector<Course> listCourses();

    int64_t insertMaterial(int64_t courseId, const std::string& title, const std::string& body);
    std::optional<Material> findMaterial(int64_t id);
    std::vector<Material> listMaterials(int64_t courseId);
    void deleteMaterial(int64_t id);

    std::vector<std::string> questionSubjects();  // distinct subjects in the question bank

private:
    Database& db_;
};
}  // namespace als
