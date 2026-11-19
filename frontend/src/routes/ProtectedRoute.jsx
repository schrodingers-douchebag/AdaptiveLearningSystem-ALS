import { Navigate, Outlet } from "react-router-dom";
import useAuth from "../hooks/useAuth.js";

export default function ProtectedRoute({ role }) {
  const { user } = useAuth();
  if (!user) return <Navigate to="/login" replace />;
  if (user.role !== role) return <Navigate to={`/${user.role}`} replace />;
  return <Outlet />;
}
