import ActivityCard from "../../components/common/ActivityCard.jsx";
import ProgressCard from "../../components/common/ProgressCard.jsx";
import TopicsCard from "../../components/common/TopicsCard.jsx";
import PlanCard from "../../components/common/PlanCard.jsx";
import SubjectsCard from "../../components/common/SubjectsCard.jsx";
import ProfilePanel from "../../components/common/ProfilePanel.jsx";

export default function Dashboard() {
  return (
    <div className="layout">
      <div className="col">
        <div className="two"><ActivityCard /><ProgressCard /></div>
        <div className="two"><TopicsCard /><PlanCard /></div>
        <SubjectsCard />
      </div>
      <ProfilePanel />
    </div>
  );
}
