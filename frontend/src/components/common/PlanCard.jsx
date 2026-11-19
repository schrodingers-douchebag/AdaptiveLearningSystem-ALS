import { useState } from "react";
import * as Icons from "lucide-react";
import { Plus, ChevronLeft, ChevronRight, Clock, ClipboardList, Users, Check, ArrowUpRight } from "lucide-react";
import { activity, progress, topics, activities, subjects } from "../../services/mockData.js";
import useKnapsack from "../../hooks/useKnapsack.js";

export default function PlanCard() {
  const [minutes, setMinutes] = useState(30);
  const { picked, totalValue } = useKnapsack(activities, minutes);
  const used = picked.reduce((s, a) => s + a.minutes, 0);
  return (
    <section className="card">
      <div className="card-head"><h2>Recommended plan</h2><span className="chip">0/1 Knapsack</span></div>
      <label className="slider">
        <span className="small muted">Time available</span>
        <input type="range" min="10" max="90" step="5" value={minutes} onChange={(e) => setMinutes(+e.target.value)} />
        <strong>{minutes} min</strong>
      </label>
      <ul className="plan-list">
        {activities.map((a) => {
          const on = picked.some((p) => p.id === a.id);
          return (
            <li key={a.id} className={on ? "picked" : ""}>
              <span className="check">{on && <Check size={12} />}</span>
              <span className="grow">{a.title}</span>
              <span className="small muted"><Clock size={12} /> {a.minutes}m</span>
              <span className="chip">P{a.priority}</span>
            </li>
          );
        })}
      </ul>
      <p className="small muted">Using {used} of {minutes} min · total priority <strong className="green">{totalValue}</strong></p>
    </section>
  );
}
