import { Outlet } from "react-router-dom";
import Sidebar from "../components/layout/Sidebar.jsx";
import Topbar from "../components/layout/Topbar.jsx";

// One shell for both roles: the sidebar switches its links by role.
export default function DashboardLayout() {
  return (
    <div className="frame">
      <Sidebar />
      <main className="main">
        <Topbar />
        <Outlet />
      </main>
    </div>
  );
}
