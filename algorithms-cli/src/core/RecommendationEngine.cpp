#include "RecommendationEngine.h"
#include "Knapsack.h"

#include <unordered_set>
#include <vector>

std::vector<Question> recommendQuestions(
    const std::vector<Question>& bank,
    const Student& student,
    int minutes
) {
    // Store completed question IDs
    std::unordered_set<int> completed(
        student.completedQuestionIds.begin(),
        student.completedQuestionIds.end()
    );

    // Exclude completed questions
    std::vector<Question> availableQuestions;

    for (const auto& question : bank) {
        if (completed.count(question.id) == 0) {
            availableQuestions.push_back(question);
        }
    }

    // Select questions within the available time
    return selectQuestions01(availableQuestions, minutes);
}