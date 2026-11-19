import { useState } from "react";
import { Navigate } from "react-router-dom";
import { Layers } from "lucide-react";
import useAuth from "../../hooks/useAuth.js";
import { errorMessage } from "../../services/api.js";

export default function Login() {
  const { user, login, register } = useAuth();
  const [mode, setMode] = useState("login"); // "login" | "register"
  const [username, setUsername] = useState("");
  const [email, setEmail] = useState("");
  const [password, setPassword] = useState("");
  const [error, setError] = useState("");
  const [busy, setBusy] = useState(false);

  if (user) return <Navigate to={`/${user.role}`} replace />;

  const submit = async (e) => {
    e.preventDefault();
    setError("");
    setBusy(true);
    try {
      if (mode === "login") await login(username.trim(), password);
      else await register(username.trim(), email.trim(), password);
      // AuthProvider now holds the user; the <Navigate> above redirects by role.
    } catch (err) {
      setError(errorMessage(err));
    } finally {
      setBusy(false);
    }
  };

  return (
    <div className="login-wrap">
      <form className="card login" onSubmit={submit}>
        <span className="logo big-logo"><Layers size={26} /></span>
        <h1>AdaptiveLearningSystem</h1>
        <p className="muted small">
          {mode === "login" ? "Sign in with your account." : "Create a student account."}
        </p>
        <input className="field" placeholder="Username" value={username} onChange={(e) => setUsername(e.target.value)} autoComplete="username" required />
        {mode === "register" && (
          <input className="field" type="email" placeholder="Email" value={email} onChange={(e) => setEmail(e.target.value)} autoComplete="email" required />
        )}
        <input className="field" type="password" placeholder="Password" value={password} onChange={(e) => setPassword(e.target.value)} autoComplete={mode === "login" ? "current-password" : "new-password"} required />
        {error && <p className="form-error small">{error}</p>}
        <button className="role-btn" type="submit" disabled={busy}>
          {busy ? "Please wait..." : mode === "login" ? "Sign in" : "Create account"}
        </button>
        <button type="button" className="link-btn small" onClick={() => { setMode(mode === "login" ? "register" : "login"); setError(""); }}>
          {mode === "login" ? "No account? Register" : "Have an account? Sign in"}
        </button>
        {mode === "login" && <p className="muted tiny">Demo: rafi / Student#123 · teacher / Teacher#123</p>}
      </form>
    </div>
  );
}
