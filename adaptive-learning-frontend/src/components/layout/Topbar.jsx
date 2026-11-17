import { Search, Mail, Bell } from "lucide-react";
import useAuth from "../../hooks/useAuth.js";

export default function Topbar() {
  const { user } = useAuth();
  return (
    <header className="topbar card">
      <div>
        <h1>Welcome back, {user.name} 👋</h1>
        <p className="muted small">Let's practise something new today!</p>
      </div>
      <div className="top-actions">
        <label className="search"><Search size={16} /><input placeholder="Search everything" /></label>
        <button className="icon-btn" aria-label="Messages"><Mail size={18} /></button>
        <button className="icon-btn" aria-label="Notifications"><Bell size={18} /></button>
      </div>
    </header>
  );
}
