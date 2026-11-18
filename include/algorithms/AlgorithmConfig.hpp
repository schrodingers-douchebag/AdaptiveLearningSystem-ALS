#pragma once

// All tunable numbers of the adaptive engine live here.
// They are sensible starting values, NOT experimentally validated.
namespace als::cfg {

// Difficulty levels
inline constexpr int kMinLevel = 1;
inline constexpr int kMaxLevel = 5;
inline constexpr int kStartLevel = 2;

// Knowledge tracking (mastery is always 0..1)
inline constexpr double kInitialMastery = 0.5;
inline constexpr double kBaseLearningRate = 0.25;
inline constexpr double kMinLearningRate = 0.05;
inline constexpr double kMaxLearningRate = 0.50;
inline constexpr double kWeakTopicThreshold = 0.6;

// Rolling windows
inline constexpr int kRecentWindow = 5;         // "recent performance" = last 5 answers
inline constexpr int kMinAttemptsToAdjust = 3;  // never move the level on 1-2 answers
inline constexpr int kRecentQuestionBlock = 5;  // do not repeat the last 5 questions
inline constexpr int kHistoryWindow = 20;       // how many past attempts the engine looks at

// Recommendation score  S(q) = wM*M + wD*D + wR*R + wN*N
inline constexpr double kWeightWeakTopic = 0.35;
inline constexpr double kWeightDifficulty = 0.30;
inline constexpr double kWeightRecent = 0.20;
inline constexpr double kWeightNovelty = 0.15;

}  // namespace als::cfg
