#include "Question.h"
#include "Knapsack.h"
#include "ActivitySelection.h"
#include "FractionalKnapsack.h"
#include "LCS.h"
#include "CoinChange.h"
#include "BellmanFord.h"
#include "FileManager.h"
#include <iostream>
#include <numeric>

int main() {
    const std::string dataDir = "data";
    ensureDataFiles(dataDir);
    const auto questions = loadQuestions(dataDir + "/questions.txt");

    std::cout << "\n=== Adaptive Learning & Question Recommendation System ===\n";
    std::cout << "1. Recommend questions (0/1 Knapsack)\n";
    std::cout << "2. Demonstrate Activity Selection\n";
    std::cout << "3. Allocate study time (Fractional Knapsack)\n";
    std::cout << "4. Compare learning patterns (LCS)\n";
    std::cout << "5. Study-time units (Coin Change)\n";
    std::cout << "6. Topic dependency paths (Bellman-Ford demo)\n";
    std::cout << "0. Exit\n";
    std::cout << "Choose: ";

    int choice;
    if (!(std::cin >> choice)) return 0;

    if (choice == 1) {
        int minutes;
        std::cout << "Available study minutes: ";
        std::cin >> minutes;
        const auto selected = selectQuestions01(questions, minutes);
        int total = 0, value = 0;
        std::cout << "\nRecommended questions:\n";
        for (const auto& q : selected) {
            std::cout << q.id << " | " << q.topic << " | " << q.estimatedMinutes
                      << " min | value " << q.score << '\n';
            total += q.estimatedMinutes; value += q.score;
        }
        std::cout << "Total: " << total << " minutes, learning value: " << value << '\n';
    } else if (choice == 2) {
        std::vector<StudyActivity> a{{1, 9, 10}, {2, 9, 11}, {3, 10, 12}, {4, 11, 12}, {5, 12, 14}};
        for (const auto& x : selectNonOverlappingActivities(a))
            std::cout << "Session " << x.id << ": " << x.start << "-" << x.finish << '\n';
    } else if (choice == 3) {
        std::vector<TopicStudy> topics{{1, 8, 30}, {2, 9, 20}, {3, 5, 15}};
        double minutes;
        std::cout << "Available minutes: "; std::cin >> minutes;
        for (const auto& x : allocateStudyTimeFractional(topics, minutes))
            std::cout << "Topic " << x.id << ": " << x.minutesAllocated << " minutes\n";
    } else if (choice == 4) {
        std::string first, second;
        std::cout << "Enter topic sequence 1 (no spaces): "; std::cin >> first;
        std::cout << "Enter topic sequence 2 (no spaces): "; std::cin >> second;
        std::cout << "Common subsequence: " << longestCommonSubsequence(first, second) << '\n';
    } else if (choice == 5) {
        int amount;
        std::cout << "Enter study minutes to represent: "; std::cin >> amount;
        const auto r = minimumCoinChange({1, 5, 10, 15}, amount);
        if (!r.possible) std::cout << "No solution.\n";
        else {
            std::cout << "Minimum units: " << r.minimumCoins << "\nUnits: ";
            for (int c : r.coinsUsed) std::cout << c << ' ';
            std::cout << '\n';
        }
    } else if (choice == 6) {
        std::vector<WeightedEdge> edges{{0,1,4},{0,2,5},{1,2,-2},{2,3,3}};
        const auto r = bellmanFord(4, edges, 0);
        if (r.hasNegativeCycle) std::cout << "Reachable negative cycle detected.\n";
        else {
            std::cout << "Shortest costs from topic 0:\n";
            for (std::size_t i = 0; i < r.distances.size(); ++i)
                std::cout << "Topic " << i << ": " << r.distances[i] << '\n';
        }
    } else if (choice != 0) {
        std::cout << "Invalid choice.\n";
    }
    std::cout << "Done.\n";
    return 0;
}
