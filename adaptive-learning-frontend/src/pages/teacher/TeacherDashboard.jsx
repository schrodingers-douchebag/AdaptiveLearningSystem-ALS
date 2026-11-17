import { useEffect, useState } from "react";
import { ArrowUpRight, ArrowDownRight } from "lucide-react";
import { getStudents } from "../../services/api.js";
import { levelColor } from "../../utils/format.js";

export default function TeacherDashboard() {
  const [rows, setRows] = useState(null);
  useEffect(() => { getStudents().then(setRows); }, []);

  return (
    <section className="card">
      <div className="card-head"><h2>Student performance</h2><span className="chip">{rows ? rows.length : 0} students</span></div>
      {!rows ? <p className="muted small">Loading students...</p> : (
        <table className="table">
          <thead><tr><th>Student</th><th>Average</th><th>Weakest topic</th><th>Attempts</th><th>Trend</th></tr></thead>
          <tbody>
            {rows.map((s) => (
              <tr key={s.id}>
                <td><strong>{s.name}</strong></td>
                <td><div className="avg"><span>{s.avg}%</span><div className="meter thin"><div className={levelColor(s.avg)} style={{ width: `${s.avg}%` }} /></div></div></td>
                <td>{s.weakest}</td>
                <td>{s.attempts}</td>
                <td>{s.trend === "up" ? <ArrowUpRight size={16} className="green" /> : <ArrowDownRight size={16} className="red" />}</td>
              </tr>
            ))}
          </tbody>
        </table>
      )}
    </section>
  );
}
