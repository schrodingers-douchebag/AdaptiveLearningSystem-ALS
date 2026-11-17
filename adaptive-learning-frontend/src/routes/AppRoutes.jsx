import { Routes, Route, Navigate } from "react-router-dom";
import ProtectedRoute from "./ProtectedRoute.jsx";
import DashboardLayout from "../layouts/DashboardLayout.jsx";
import Login from "../pages/auth/Login.jsx";
import Dashboard from "../pages/student/Dashboard.jsx";
import Topics from "../pages/student/Topics.jsx";
import StudyPlan from "../pages/student/StudyPlan.jsx";
import TeacherDashboard from "../pages/teacher/TeacherDashboard.jsx";

export default function AppRoutes() {
  return (
    <Routes>
      <Route path="/login" element={<Login />} />

      <Route path="/student" element={<ProtectedRoute role="student" />}>
        <Route element={<DashboardLayout />}>
          <Route index element={<Dashboard />} />
          <Route path="topics" element={<Topics />} />
          <Route path="plan" element={<StudyPlan />} />
        </Route>
      </Route>

      <Route path="/teacher" element={<ProtectedRoute role="teacher" />}>
        <Route element={<DashboardLayout />}>
          <Route index element={<TeacherDashboard />} />
        </Route>
      </Route>

      <Route path="*" element={<Navigate to="/login" replace />} />
    </Routes>
  );
}
