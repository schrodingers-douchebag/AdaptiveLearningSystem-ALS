import { useEffect, useState } from "react";
import { Link, useParams } from "react-router-dom";
import { ArrowLeft, Trash2 } from "lucide-react";
import useAuth from "../../hooks/useAuth.js";
import { getCourse, addMaterial, deleteMaterial, importQuestions, errorMessage } from "../../services/api.js";

const MAX_MATERIAL = 200 * 1024; // keep in sync with the backend limits
const MAX_IMPORT = 300 * 1024;

const FORMAT_HELP = `TOPIC: Arrays
DIFFICULTY: 2
Q: What is the index of the first element of a C array?
A) 1
B) 0
C) -1
ANSWER: B

Q: Which structure is LIFO?
A) Queue
B) Stack
ANSWER: B`;

// One course: its study materials (everyone) + upload tools (the course's teacher).
export default function CourseDetail() {
  const { id } = useParams();
  const { user } = useAuth();
  const [data, setData] = useState(null); // {course, materials}
  const [error, setError] = useState("");
  const [notice, setNotice] = useState("");
  const [matTitle, setMatTitle] = useState("");
  const [matBody, setMatBody] = useState("");
  const [busy, setBusy] = useState(false);

  useEffect(() => { getCourse(id).then(setData).catch((e) => setError(errorMessage(e))); }, [id]);

  const canEdit = !!data && user.role === "teacher" && user.id === data.course.teacher_id;
  const reload = () => getCourse(id).then(setData).catch((e) => setError(errorMessage(e)));

  // Read a chosen .txt file in the browser and hand its text to `onText`.
  const readTxt = async (file, limit, onText) => {
    setError(""); setNotice("");
    if (!file) return;
    if (file.size > limit) { setError(`File is too large (max ${Math.round(limit / 1024)} KB).`); return; }
    try { await onText(await file.text(), file); } catch (e) { setError(errorMessage(e)); }
  };

  const onMaterialFile = (e) => {
    const file = e.target.files[0];
    e.target.value = "";
    readTxt(file, MAX_MATERIAL, (text, f) => {
      setMatBody(text);
      if (!matTitle) setMatTitle(f.name.replace(/\.txt$/i, ""));
    });
  };

  const saveMaterial = async (e) => {
    e.preventDefault();
    setError(""); setNotice(""); setBusy(true);
    try {
      await addMaterial(id, matTitle.trim(), matBody);
      setMatTitle(""); setMatBody(""); setNotice("Material added.");
      await reload();
    } catch (err) { setError(errorMessage(err)); }
    finally { setBusy(false); }
  };

  const removeMaterial = async (mid) => {
    if (!window.confirm("Delete this material?")) return;
    setError(""); setNotice("");
    try { await deleteMaterial(mid); await reload(); } catch (err) { setError(errorMessage(err)); }
  };

  const onQuestionFile = (e) => {
    const file = e.target.files[0];
    e.target.value = "";
    readTxt(file, MAX_IMPORT, async (text) => {
      setBusy(true);
      try {
        const n = await importQuestions(id, text);
        setNotice(`Imported ${n} question${n === 1 ? "" : "s"}.`);
        await reload();
      } finally { setBusy(false); }
    });
  };

  if (!data) return <div className="page-narrow"><section className="card">{error ? <p className="form-error small">{error}</p> : <p className="muted small">Loading course...</p>}</section></div>;

  const { course, materials } = data;
  return (
    <div className="page-narrow stack">
      <Link to={`/${user.role}/courses`} className="link-btn small back"><ArrowLeft size={14} /> All courses</Link>
      <section className="card">
        <div className="card-head"><h2>{course.title}</h2><span className="chip">{course.question_count} questions</span></div>
        <p className="small muted">{course.description || "No description."} · Teacher: {course.teacher}</p>
        {error && <p className="form-error small">{error}</p>}
        {notice && <p className="green small">{notice}</p>}
      </section>

      <section className="card stack">
        <div className="card-head"><h3>Study materials</h3><span className="chip">{materials.length}</span></div>
        {materials.length === 0 && <p className="muted small">No materials yet.</p>}
        {materials.map((m) => (
          <article key={m.id} className="material">
            <div className="row-between">
              <strong>{m.title}</strong>
              {canEdit && <button className="sq soft" onClick={() => removeMaterial(m.id)} aria-label="Delete material"><Trash2 size={14} /></button>}
            </div>
            <span className="tiny muted">{m.created_at}</span>
            <pre className="material-body">{m.body}</pre>
          </article>
        ))}
      </section>

      {canEdit && (
        <>
          <form className="card stack" onSubmit={saveMaterial}>
            <div className="card-head"><h3>Add study material</h3></div>
            <label className="small muted">Choose a .txt file (or type below)</label>
            <input type="file" accept=".txt,text/plain" onChange={onMaterialFile} />
            <input className="field" placeholder="Title" value={matTitle} onChange={(e) => setMatTitle(e.target.value)} required />
            <textarea className="field area" placeholder="Material text" value={matBody} onChange={(e) => setMatBody(e.target.value)} required />
            <button className="role-btn" type="submit" disabled={busy}>Save material</button>
          </form>

          <section className="card stack">
            <div className="card-head"><h3>Import questions from a .txt file</h3></div>
            <p className="small muted">Questions are added to this course and used by the adaptive practice. If any block is invalid, nothing is saved.</p>
            <input type="file" accept=".txt,text/plain" onChange={onQuestionFile} disabled={busy} />
            <details>
              <summary className="small">File format</summary>
              <pre className="material-body">{FORMAT_HELP}</pre>
              <p className="tiny muted">Separate questions with a blank line. TOPIC defaults to General and DIFFICULTY (1-5) to 2. Lines starting with # are ignored.</p>
            </details>
          </section>
        </>
      )}
    </div>
  );
}
