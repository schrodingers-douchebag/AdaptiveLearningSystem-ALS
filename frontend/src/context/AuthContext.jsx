import { createContext, useState } from 'react';
import { login as apiLogin, register as apiRegister, TOKEN_KEY, USER_KEY } from '../services/api.js';

export const AuthContext = createContext(null);

// Turn the backend user ({id, username, email, role}) into what the UI expects.
function toUiUser(u) {
  const isTeacher = u.role === 'teacher';
  return {
    id: u.id,
    name: u.username,
    email: u.email,
    role: u.role,
    roleLabel: isTeacher ? 'Teacher' : 'CSE Student',
    avatar: `https://api.dicebear.com/10.x/bottts-neutral/svg?seed=${encodeURIComponent(u.username)}`,
  };
}

function save({ token, user }) {
  const ui = toUiUser(user);
  localStorage.setItem(TOKEN_KEY, token);
  localStorage.setItem(USER_KEY, JSON.stringify(ui));
  return ui;
}

export function AuthProvider({ children }) {
  const [user, setUser] = useState(() => {
    try {
      return localStorage.getItem(TOKEN_KEY) ? JSON.parse(localStorage.getItem(USER_KEY)) : null;
    } catch {
      return null;
    }
  });

  // Both throw on failure; the caller shows the error.
  const login = async (username, password) => setUser(save(await apiLogin(username, password)));
  const register = async (username, email, password) =>
    setUser(save(await apiRegister(username, email, password)));
  const logout = () => {
    localStorage.removeItem(TOKEN_KEY);
    localStorage.removeItem(USER_KEY);
    setUser(null);
  };

  return <AuthContext.Provider value={{ user, login, register, logout }}>{children}</AuthContext.Provider>;
}
