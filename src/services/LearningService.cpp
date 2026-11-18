#include "services/LearningService.hpp"

#include "algorithms/AlgorithmConfig.hpp"
#include "utils/AppError.hpp"
#include "utils/Validator.hpp"

namespace als {

StartResult LearningService::start(int64_t userId) {
    StartResult r;
    r.level = progress_.ensureProfile(userId, cfg::kStartLevel).currentLevel;
    r.pick = recommendation_.next(userId);
    if (r.pick) r.question = questions_.findById(r.pick->questionId);
    return r;
}

SubmitResult LearningService::submitAnswer(int64_t userId, int64_t questionId, int64_t optionId, int timeTaken) {
    auto q = questions_.findById(questionId);
    if (!q) throw AppError(404, "Question not found");

    const AnswerOption* chosen = nullptr;
    const AnswerOption* correct = nullptr;
    for (const auto& o : q->options) {
        if (o.id == optionId) chosen = &o;
        if (o.isCorrect) correct = &o;
    }
    if (!chosen) throw AppError(400, "selected_option_id does not belong to this question");
    if (!correct) throw AppError(500, "Question has no correct option");
    timeTaken = Validator::clamp(timeTaken, 0, 3600);

    // Everything below must succeed together, or nothing is saved.
    Transaction tx(db_);

    StudentState state = recommendation_.buildState(userId);
    AnswerEvent event{q->id, q->topic, q->difficulty, chosen->isCorrect, timeTaken};
    EngineResult res = engine_.processAnswer(state, event, questions_.listAll());

    Answer a;
    a.userId = userId;
    a.questionId = q->id;
    a.selectedOptionId = chosen->id;
    a.isCorrect = chosen->isCorrect;
    a.timeTaken = timeTaken;
    answers_.insert(a);

    LearningProgress p = progress_.get(userId, q->topic).value_or(LearningProgress{});
    p.userId = userId;
    p.topic = q->topic;
    p.masteryScore = res.updatedTopic.mastery;
    p.totalAttempts = res.updatedTopic.attempts;
    p.correctAttempts += chosen->isCorrect ? 1 : 0;
    progress_.upsert(p);
    progress_.setLevel(userId, res.newLevel);
    if (res.next) recs_.save(userId, res.next->questionId, res.next->score);
    tx.commit();

    SubmitResult out;
    out.isCorrect = chosen->isCorrect;
    out.feedback = chosen->isCorrect ? "Correct answer!" : "Not quite. The correct answer is: " + correct->text;
    out.correctOptionId = correct->id;
    out.topic = q->topic;
    out.updatedMastery = res.updatedTopic.mastery;
    out.previousDifficulty = res.previousLevel;
    out.nextDifficulty = res.newLevel;
    out.levelReason = res.levelReason;
    out.next = res.next;
    out.summary = res.summary;
    if (res.next) out.nextQuestion = questions_.findById(res.next->questionId);
    return out;
}

std::vector<LearningProgress> LearningService::progress(int64_t userId) { return progress_.all(userId); }

int LearningService::level(int64_t userId) { return progress_.ensureProfile(userId, cfg::kStartLevel).currentLevel; }

}  // namespace als
