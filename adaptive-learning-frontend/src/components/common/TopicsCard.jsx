import { useState } from "react";
import * as Icons from "lucide-react";
import { Plus, ChevronLeft, ChevronRight, Clock, ClipboardList, Users, Check, ArrowUpRight } from "lucide-react";
import { activity, progress, topics, activities, subjects } from "../../services/mockData.js";

export default function TopicsCard() {
  const sorted = [...topics].sort((a, b) => a.accuracy - b.accuracy);
  return (
    <section className="card">
      <div className="card-head"><h2>Topic performance</h2><span className="chip warn">Weakest first</span></div>
      <ul className="topic-list">
        {sorted.map((t) => (
          <li key={t.name}>
            <div className="row-between"><span>{t.name}</span><strong className={t.accuracy < 50 ? "red" : t.accuracy < 70 ? "amber" : "green"}>{t.accuracy}%</strong></div>
            <div className="meter"><div style={{ width: `${t.accuracy}%` }} className={t.accuracy < 50 ? "low" : t.accuracy < 70 ? "mid" : "high"} /></div>
            <span className="small muted">{t.subject}</span>
          </li>
        ))}
      </ul>
    </section>
  );
}
