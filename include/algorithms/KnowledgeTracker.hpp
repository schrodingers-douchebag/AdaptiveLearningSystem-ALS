#pragma once

namespace als {

struct TopicMastery {
    double mastery = 0.5;  // 0..1
    int attempts = 0;
};

// Keeps a per-topic mastery score using an exponential moving average.
// Correct answers on hard questions move the score more; wrong answers on easy questions hurt more.
class KnowledgeTracker {
public:
    static double learningRate(bool correct, int difficulty);
    static double update(double mastery, bool correct, int difficulty);
};

}  // namespace als
