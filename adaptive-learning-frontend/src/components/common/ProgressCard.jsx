import { useState } from "react";
import * as Icons from "lucide-react";
import { Plus, ChevronLeft, ChevronRight, Clock, ClipboardList, Users, Check, ArrowUpRight } from "lucide-react";
import { activity, progress, topics, activities, subjects } from "../../services/mockData.js";
import Ring from "../ui/Ring.jsx";

export default function ProgressCard() {
  const stats = [
    { n: progress.completed, l: "Quizzes done", c: "#7c3aed", i: <Check size={16} /> },
    { n: progress.inProgress, l: "Topics in progress", c: "#c2571a", i: <Icons.RefreshCw size={16} /> },
    { n: progress.upcoming, l: "Upcoming", c: "#dc2f3c", i: <Icons.CalendarDays size={16} /> },
  ];
  return (
    <section className="card">
      <div className="card-head"><h2>Progress</h2><select className="select"><option>This week</option></select></div>
      <div className="progress-grid">
        <div className="tile ring-tile">
          <span className="small muted">Progress</span>
          <Ring percent={progress.percent} />
          <div className="last-week"><strong>{progress.lastWeek}%</strong><span className="small muted">Last week</span></div>
        </div>
        <div className="stat-list">
          {stats.map((s) => (
            <div key={s.l} className="tile stat">
              <span className="dot" style={{ background: s.c }}>{s.i}</span>
              <div><strong>{s.n}</strong><div className="small muted">{s.l}</div></div>
            </div>
          ))}
        </div>
      </div>
    </section>
  );
}
