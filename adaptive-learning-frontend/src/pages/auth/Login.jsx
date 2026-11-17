import { Navigate, useNavigate } from "react-router-dom";
import { Layers, GraduationCap, Presentation } from "lucide-react";
import useAuth from "../../hooks/useAuth.js";

export default function Login() {
  const { user, login } = useAuth();
  const navigate = useNavigate();
  if (user) return <Navigate to={`/${user.role}`} replace />;

  const enter = (role) => { login(role); navigate(`/${role}`); };

  return (
    <div className="login-wrap">
      <div className="card login">
        <span className="logo big-logo"><Layers size={26} /></span>
        <h1>AdaptiveLearningSystem</h1>
        <p className="muted small">Demo mode. Pick a role to look around, no password needed.</p>
        <button className="role-btn" onClick={() => enter("student")}><GraduationCap size={20} /> Continue as student</button>
        <button className="role-btn" onClick={() => enter("teacher")}><Presentation size={20} /> Continue as teacher</button>
      </div>
    </div>
  );
}
