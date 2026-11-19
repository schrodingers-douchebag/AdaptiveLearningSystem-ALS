import { useState } from "react";
import { Plus, ChevronLeft, ChevronRight, MoreVertical, BadgeCheck, Check } from "lucide-react";
import useAuth from "../../hooks/useAuth.js";
import { week, today, todos } from "../../services/mockData.js";

export default function ProfilePanel() {
  const { user } = useAuth();
  const [items, setItems] = useState(todos);
  const toggle = (id) => setItems((l) => l.map((t) => (t.id === id ? { ...t, done: !t.done } : t)));
  const letters = ["M", "T", "W", "T", "F", "S", "S"];
  return (
    <aside className="card profile">
      <div className="card-head"><h2>Profile</h2><button className="sq"><MoreVertical size={16} /></button></div>
      <div className="avatar-wrap"><img src={user.avatar} alt={user.name} /></div>
      <div className="center"><strong>{user.name} <BadgeCheck size={14} className="verified" /></strong><div className="small muted">{user.roleLabel}</div></div>

      <div className="calendar">
        <div className="row-between cal-head"><ChevronLeft size={16} /><strong>November</strong><ChevronRight size={16} /></div>
        <div className="cal-days">
          {week.map((d, i) => (
            <div key={d} className={"cal-day" + (d === today ? " today" : "")}>
              <span className="small muted">{letters[i]}</span><strong>{d}</strong>
            </div>
          ))}
        </div>
      </div>

      <div className="card-head"><h3>To do list</h3><button className="sq soft"><Plus size={16} /></button></div>
      <div className="todo-tabs small"><b>All <i>{items.length}</i></b><span>Open</span><span>Closed</span></div>
      {items.map((t) => (
        <div key={t.id} className="todo">
          <div className="row-between">
            <div><strong className={"small" + (t.done ? " strike" : "")}>{t.title}</strong><div className="tiny muted">{t.note}</div></div>
            <button className={"tick" + (t.done ? " on" : "")} onClick={() => toggle(t.id)} aria-label="Toggle done">{t.done && <Check size={12} />}</button>
          </div>
          <div className="tiny muted todo-date">{t.date}</div>
        </div>
      ))}
    </aside>
  );
}
