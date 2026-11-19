import { useRef, useState } from "react";
import { startLearning, submitAnswer, errorMessage } from "../../services/api.js";

const nowMs = () => Date.now(); // kept outside the component (React purity lint)

// Adaptive quiz loop: POST /learning/start, then POST /learning/submit-answer
// (the server returns feedback AND the next recommended question each time).
export default function Practice() {
  const [question, setQuestion] = useState(null);
  const [level, setLevel] = useState(null);
  const [result, setResult] = useState(null);   // feedback for the answered question
  const [picked, setPicked] = useState(null);
  const [started, setStarted] = useState(false);
  const [busy, setBusy] = useState(false);
  const [error, setError] = useState("");
  const shownAt = useRef(0);

  const show = (q) => { setQuestion(q); setResult(null); setPicked(null); shownAt.current = nowMs(); };

  const start = async () => {
    setError(""); setBusy(true);
    try {
      const r = await startLearning();
      setLevel(r.level); setStarted(true); show(r.question);
    } catch (e) { setError(errorMessage(e)); }
    finally { setBusy(false); }
  };

  const answer = async (optionId) => {
    if (result || busy) return;
    setError(""); setBusy(true); setPicked(optionId);
    try {
      const seconds = Math.max(1, Math.round((nowMs() - shownAt.current) / 1000));
      setResult(await submitAnswer(question.id, optionId, seconds));
    } catch (e) { setError(errorMessage(e)); setPicked(null); }
    finally { setBusy(false); }
  };

  const optionClass = (o) => {
    if (!result) return "option" + (picked === o.id ? " selected" : "");
    if (o.id === result.correct_option_id) return "option correct";
    if (o.id === picked) return "option wrong";
    return "option";
  };

  return (
    <div className="page-narrow">
      <section className="card">
        <div className="card-head">
          <h2>Practice</h2>
          {level != null && <span className="chip">Difficulty {result ? result.next_difficulty : level}</span>}
        </div>
        {error && <p className="form-error small">{error}</p>}

        {!started && <button className="role-btn" onClick={start} disabled={busy}>{busy ? "Loading..." : "Start practice"}</button>}
        {started && !question && <p className="muted small">No more questions available right now.</p>}

        {question && (
          <>
            <p className="small muted">{question.subject} · {question.topic}</p>
            <h3 className="q-text">{question.question_text}</h3>
            <div className="options">
              {question.options.map((o) => (
                <button key={o.id} className={optionClass(o)} onClick={() => answer(o.id)} disabled={!!result || busy}>{o.text}</button>
              ))}
            </div>
            {result && (
              <div className="feedback">
                <strong className={result.is_correct ? "green" : "red"}>{result.is_correct ? "Correct" : "Not quite"}</strong>
                <p className="small">{result.feedback}</p>
                <p className="small muted">{result.topic} mastery: {Math.round(result.updated_mastery * 100)}%</p>
                <button className="role-btn" onClick={() => show(result.next_question)}>Next question</button>
              </div>
            )}
          </>
        )}
      </section>
    </div>
  );
}
