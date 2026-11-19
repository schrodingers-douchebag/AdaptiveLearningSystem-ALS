#pragma once
#include <cstdint>
#include <string>

namespace als {

// A course groups study materials and questions. Questions belong to a course through
// their `subject` field (subject == course title), so the adaptive engine needs no changes.
struct Course {
    int64_t id = 0;
    std::string title;
    std::string description;
    int64_t teacherId = 0;
    std::string teacherName;
    std::string createdAt;
    int materialCount = 0;
    int questionCount = 0;
};

// A text document (course notes, an exam guide...) uploaded by the course's teacher.
struct Material {
    int64_t id = 0;
    int64_t courseId = 0;
    std::string title;
    std::string body;
    std::string createdAt;
};

}  // namespace als
