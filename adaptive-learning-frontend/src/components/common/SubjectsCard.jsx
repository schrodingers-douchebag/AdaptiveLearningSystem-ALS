import { useState } from "react";
import * as Icons from "lucide-react";
import { Plus, ChevronLeft, ChevronRight, Clock, ClipboardList, Users, Check, ArrowUpRight } from "lucide-react";
import { activity, progress, topics, activities, subjects } from "../../services/mockData.js";

export default function SubjectsCard() {
  const [tab, setTab] = useState("All");
  const list = subjects.filter((s) => tab === "All" || (tab === "Active" ? s.finish < 100 : s.finish === 100));
  return (
    <section className="card wide">
      <div className="card-head">
        <h2>Subjects in progress</h2>
        <button className="btn-soft">Add new <Plus size={16} /></button>
      </div>
      <div className="tabs-row">
        <div className="tabs">{["All", "Active", "Completed"].map((t) => <button key={t} className={tab === t ? "on" : ""} onClick={() => setTab(t)}>{t}</button>)}</div>
        <div className="arrows"><button className="sq"><ChevronLeft size={16} /></button><button className="sq"><ChevronRight size={16} /></button></div>
      </div>
      <div className="course-grid">
        {list.map((s) => (
          <article key={s.code} className="course">
            <div className="course-top"><span className="code">{s.code}</span><div><strong>{s.title}</strong><div className="small muted">{s.level}</div></div></div>
            <div className="meta small"><span><ClipboardList size={13} /> {s.quizzes}</span><span><Clock size={13} /> {s.minutes}</span><span><Users size={13} /> {s.students}</span></div>
            <div className="small strong">{s.finish}% Finish</div>
            <div className="meter thin"><div className="high" style={{ width: `${s.finish}%` }} /></div>
            <div className="row-between small muted"><span>{s.lessons}</span><span>{s.left}</span></div>
            <div className="row-between teacher">
              <div className="who"><img src={s.avatar} alt="" /><div><strong className="small">{s.teacher}</strong><div className="tiny muted">Teacher</div></div></div>
              <button className="btn-outline">Follow</button>
            </div>
          </article>
        ))}
      </div>
    </section>
  );
}
