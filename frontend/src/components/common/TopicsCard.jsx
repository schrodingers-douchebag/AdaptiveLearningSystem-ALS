import { useEffect, useState } from "react";
import { getWeakTopics, errorMessage } from "../../services/api.js";

// Live data: GET /api/analytics/weak-topics (already sorted weakest first by the server).
export default function TopicsCard() {
  const [topics, setTopics] = useState(null);
  const [error, setError] = useState("");

  useEffect(() => {
    getWeakTopics().then(setTopics).catch((e) => setError(errorMessage(e)));
  }, []);

  return (
    <section className="card">
      <div className="card-head"><h2>Topic performance</h2><span className="chip warn">Weakest first</span></div>
      {error && <p className="form-error small">{error}</p>}
      {!error && !topics && <p className="muted small">Loading topics...</p>}
      {topics && topics.length === 0 && <p className="muted small">No answers yet. Open Practice and answer a few questions.</p>}
      {topics && topics.length > 0 && (
        <ul className="topic-list">
          {topics.map((t) => {
            const pct = Math.round(t.mastery_score * 100);
            return (
              <li key={t.topic}>
                <div className="row-between"><span>{t.topic}</span><strong className={pct < 50 ? "red" : pct < 70 ? "amber" : "green"}>{pct}%</strong></div>
                <div className="meter"><div style={{ width: `${pct}%` }} className={pct < 50 ? "low" : pct < 70 ? "mid" : "high"} /></div>
                <span className="small muted">{t.total_attempts} attempts · {t.correct_attempts} correct</span>
              </li>
            );
          })}
        </ul>
      )}
    </section>
  );
}
