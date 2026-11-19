import { useEffect, useState } from "react";
import { Link } from "react-router-dom";
import { BookOpen, FileText, HelpCircle } from "lucide-react";
import useAuth from "../../hooks/useAuth.js";
import { getCourses, createCourse, errorMessage } from "../../services/api.js";

// Course list for both roles. Teachers also get a "New course" form.
export default function Courses() {
  const { user } = useAuth();
  const [courses, setCourses] = useState(null);
  const [error, setError] = useState("");
  const [title, setTitle] = useState("");
  const [description, setDescription] = useState("");
  const [busy, setBusy] = useState(false);

  useEffect(() => { getCourses().then(setCourses).catch((e) => setError(errorMessage(e))); }, []);

  const add = async (e) => {
    e.preventDefault();
    setError(""); setBusy(true);
    try {
      const c = await createCourse(title.trim(), description.trim());
      setCourses((list) => [...(list || []), c]);
      setTitle(""); setDescription("");
    } catch (err) { setError(errorMessage(err)); }
    finally { setBusy(false); }
  };

  return (
    <div className="page-narrow stack">
      <section className="card">
        <div className="card-head"><h2>Courses</h2><span className="chip">{courses ? courses.length : 0}</span></div>
        {error && <p className="form-error small">{error}</p>}
        {!error && !courses && <p className="muted small">Loading courses...</p>}
        {courses && courses.length === 0 && <p className="muted small">No courses yet.</p>}
        <ul className="course-list">
          {(courses || []).map((c) => (
            <li key={c.id}>
              <Link to={`/${user.role}/courses/${c.id}`} className="course-item">
                <span className="course-icon"><BookOpen size={18} /></span>
                <span className="grow">
                  <strong>{c.title}</strong>
                  <span className="small muted block">{c.description || `Teacher: ${c.teacher}`}</span>
                </span>
                <span className="small muted"><FileText size={13} /> {c.material_count}</span>
                <span className="small muted"><HelpCircle size={13} /> {c.question_count}</span>
              </Link>
            </li>
          ))}
        </ul>
      </section>

      {user.role === "teacher" && (
        <form className="card stack" onSubmit={add}>
          <div className="card-head"><h2>New course</h2></div>
          <input className="field" placeholder="Course title (3-120 characters)" value={title} onChange={(e) => setTitle(e.target.value)} required />
          <input className="field" placeholder="Short description (optional)" value={description} onChange={(e) => setDescription(e.target.value)} />
          <button className="role-btn" type="submit" disabled={busy}>{busy ? "Creating..." : "Create course"}</button>
        </form>
      )}
    </div>
  );
}
