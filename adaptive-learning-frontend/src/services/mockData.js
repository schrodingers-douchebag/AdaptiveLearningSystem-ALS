export const user = {
  name: 'Rafi',
  role: 'CSE Student',
  avatar: 'https://robohash.org/student01.png?size=150x150',
};

export const studentNav = [
  { to: '/student', label: 'Dashboard', icon: 'LayoutGrid', end: true },
  { to: '/student/topics', label: 'Topics', icon: 'BookOpen' },
  { to: '/student/plan', label: 'Study plan', icon: 'CalendarCheck' },
];

export const teacherNav = [{ to: '/teacher', label: 'Students', icon: 'Users', end: true }];

export const students = [
  { id: 1, name: 'Rafi', avg: 73, weakest: 'Graphs', attempts: 14, trend: 'up' },
  { id: 2, name: 'Apurbo Tonoy', avg: 81, weakest: 'Recursion', attempts: 18, trend: 'up' },
  { id: 3, name: 'Jannat', avg: 56, weakest: 'Linked lists', attempts: 9, trend: 'down' },
  { id: 4, name: 'Anup', avg: 67, weakest: 'Stacks', attempts: 12, trend: 'up' },
  { id: 5, name: 'Tonmoy', avg: 49, weakest: 'Graphs', attempts: 6, trend: 'down' },
];

export const activity = [
  { day: 'Mon', hours: 2.5 },
  { day: 'Tue', hours: 2.2 },
  { day: 'Wed', hours: 3.0 },
  { day: 'Thu', hours: 4.0 },
  { day: 'Fri', hours: 1.6 },
  { day: 'Sat', hours: 4.0 },
  { day: 'Sun', hours: 3.5 },
];

export const progress = { percent: 73, lastWeek: 20, completed: 8, inProgress: 12, upcoming: 14 };

// topic accuracy from quizzes (drives "weak topics")
export const topics = [
  { name: 'Linked lists', accuracy: 42, subject: 'Data Structures' },
  { name: 'Graphs', accuracy: 38, subject: 'Algorithms' },
  { name: 'Stacks', accuracy: 61, subject: 'Data Structures' },
  { name: 'Arrays', accuracy: 88, subject: 'Data Structures' },
  { name: 'Recursion', accuracy: 74, subject: 'Algorithms' },
];

// candidate practice activities -> items for 0/1 knapsack
export const activities = [
  { id: 1, title: 'Array practice', minutes: 10, priority: 5 },
  { id: 2, title: 'Linked-list practice', minutes: 15, priority: 9 },
  { id: 3, title: 'Stack practice', minutes: 10, priority: 7 },
  { id: 4, title: 'Graph practice', minutes: 20, priority: 10 },
  { id: 5, title: 'Recursion drills', minutes: 12, priority: 6 },
];

export const subjects = [
  {
    code: 'SP',
    title: 'Structured Programming',
    level: 'Beginner level',
    quizzes: 12,
    minutes: 90,
    students: 52,
    finish: 25,
    lessons: '18/40 Lessons',
    left: '2 hours left',
    teacher: 'Pulok Sir',
    avatar: 'https://api.dicebear.com/10.x/bottts-neutral/svg?seed=Pulok Sir',
  },
  {
    code: 'DS',
    title: 'Data Structures',
    level: 'Intermediate level',
    quizzes: 16,
    minutes: 30,
    students: 48,
    finish: 45,
    lessons: '5/10 Lessons',
    left: '2 hours left',
    teacher: 'i forgot',
    avatar: 'https://api.dicebear.com/10.x/bottts-neutral/svg',
  },
  {
    code: 'AL',
    title: 'Algorithm Design',
    level: 'Advanced level',
    quizzes: 20,
    minutes: 90,
    students: 61,
    finish: 88,
    lessons: '16/20 Lessons',
    left: '2 hours left',
    teacher: 'Pulok Sir',
    avatar: 'https://api.dicebear.com/10.x/shapes/svg',
  },
];

export const week = [16, 17, 18, 19, 20, 21, 22];
export const today = 18;

export const todos = [
  {
    id: 1,
    title: 'Finish diagnostic quiz',
    note: 'Takes about 20 minutes',
    date: 'November 19, 2026',
    done: true,
  },
  {
    id: 2,
    title: 'Analyze your quiz results',
    note: 'Review your scores and focus on weak areas.',
    date: 'November 21, 2026',
    done: false,
  },
];
