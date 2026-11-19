import { NavLink } from "react-router-dom";
import * as Icons from "lucide-react";
import { Layers, LogOut } from "lucide-react";
import useAuth from "../../hooks/useAuth.js";
import { studentNav, teacherNav } from "../../services/mockData.js";

export default function Sidebar() {
  const { user, logout } = useAuth();
  const items = user.role === "teacher" ? teacherNav : studentNav;
  return (
    <aside className="sidebar">
      <div className="brand"><span className="logo"><Layers size={20} /></span>Adaptive</div>
      <nav>
        {items.map((n) => {
          const I = Icons[n.icon] || Icons.Circle;
          return (
            <NavLink key={n.to} to={n.to} end={n.end} className={({ isActive }) => "nav-item" + (isActive ? " active" : "")}>
              <I size={18} strokeWidth={1.8} /> <span>{n.label}</span>
            </NavLink>
          );
        })}
        <button className="nav-item" onClick={logout}><LogOut size={18} strokeWidth={1.8} /> <span>Log out</span></button>
      </nav>
      <div className="promo">
        <div className="promo-art" aria-hidden>🎓</div>
        <strong>Try the Algo Lab</strong>
        <p>See Knapsack and Activity Selection work step by step.</p>
        <button className="btn-accent">Open lab</button>
      </div>
    </aside>
  );
}
