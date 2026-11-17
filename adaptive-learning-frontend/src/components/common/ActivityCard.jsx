import { useState } from 'react';
import * as Icons from 'lucide-react';
import {
  Plus,
  ChevronLeft,
  ChevronRight,
  Clock,
  ClipboardList,
  Users,
  Check,
  ArrowUpRight,
} from 'lucide-react';
import { activity, progress, topics, activities, subjects } from '../../services/mockData.js';

export default function ActivityCard() {
  const max = 4.5;
  return (
    <section className="card">
      <div className="card-head">
        <h2>Activity</h2>
        <select className="select">
          <option>This week</option>
          <option>Last week</option>
        </select>
      </div>
      <span className="trend">
        <ArrowUpRight size={14} />
      </span>{' '}
      <span className="small green">+3% Increase than last week</span>
      <div className="big">
        3,5h <span className="muted small">Hours spent</span>
      </div>
      <div className="bars">
        {activity.map((a) => (
          <div key={a.day} className="bar-col">
            <div className="bar-track">
              <div
                className="bar-fill"
                style={{ height: `${(a.hours / max) * 100}%` }}
                title={`${a.hours}h`}
              />
            </div>
            <span className="small muted">{a.day}</span>
          </div>
        ))}
      </div>
    </section>
  );
}
