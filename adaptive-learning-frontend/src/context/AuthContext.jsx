import { createContext, useState } from 'react';

export const AuthContext = createContext(null);

const DEMO_USERS = {
  student: {
    name: 'Rafi',
    role: 'student',
    roleLabel: 'CSE Student',
    avatar: 'https://robohash.org/student01.png?size=150x150',
  },
  teacher: {
    name: 'Pulok Sir',
    role: 'teacher',
    roleLabel: 'Teacher',
    avatar: 'https://robohash.org/teacher01.png?size=150x150',
  },
};

export function AuthProvider({ children }) {
  const [user, setUser] = useState(() => {
    try {
      return JSON.parse(localStorage.getItem('als_user'));
    } catch {
      return null;
    }
  });

  const login = (role) => {
    const u = DEMO_USERS[role];
    setUser(u);
    localStorage.setItem('als_user', JSON.stringify(u));
  };
  const logout = () => {
    setUser(null);
    localStorage.removeItem('als_user');
  };

  return <AuthContext.Provider value={{ user, login, logout }}>{children}</AuthContext.Provider>;
}
