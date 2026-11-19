#pragma once
#include <nlohmann/json.hpp>
#include "models/Course.hpp"

namespace als {
inline nlohmann::json toJson(const Course& c) {
    return {{"id", c.id}, {"title", c.title}, {"description", c.description}, {"teacher_id", c.teacherId},
            {"teacher", c.teacherName}, {"material_count", c.materialCount}, {"question_count", c.questionCount},
            {"created_at", c.createdAt}};
}
inline nlohmann::json toJson(const Material& m) {
    return {{"id", m.id}, {"course_id", m.courseId}, {"title", m.title}, {"body", m.body}, {"created_at", m.createdAt}};
}
}  // namespace als
