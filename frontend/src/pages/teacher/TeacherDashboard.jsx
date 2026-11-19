import { useEffect, useState } from "react";
import { getStudents, errorMessage } from "../../services/api.js";
import { levelColor } from "../../utils/format.js";

// Live data: GET /api/analytics/students (teacher token required).
export default function TeacherDashboard() {
  const [rows, setRows] = useState(null);
  const [error, setError] = useState("");
  useEffect(() => { getStudents().then(setRows).catch((e) => setError(errorMessage(e))); }, []);

  return (
    <section className="card">
      <div className="card-head"><h2>Student performance</h2><span className="chip">{rows ? rows.length : 0} students</span></div>
      {error && <p className="form-error small">{error}</p>}
      {!error && !rows && <p className="muted small">Loading students...</p>}
      {rows && (
        <table className="table">
          <thead><tr><th>Student</th><th>Accuracy</th><th>Weakest topic</th><th>Attempts</th><th>Level</th></tr></thead>
          <tbody>
            {rows.map((s) => {
              const pct = Math.round(s.accuracy * 100);
              return (
                <tr key={s.id}>
                  <td><strong>{s.name}</strong></td>
                  <td><div className="avg"><span>{pct}%</span><div className="meter thin"><div className={levelColor(pct)} style={{ width: `${pct}%` }} /></div></div></td>
                  <td>{s.weakest_topic || "-"}</td>
                  <td>{s.attempts}</td>
                  <td>{s.level}</td>
                </tr>
              );
            })}
          </tbody>
        </table>
      )}
    </section>
  );
}
