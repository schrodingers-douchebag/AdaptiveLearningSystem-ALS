import axios from "axios";

// Backend base URL comes from .env (VITE_API_URL). Falls back to the backend default port.
export const API_URL = import.meta.env.VITE_API_URL || "http://localhost:8080/api";

export const TOKEN_KEY = "als_token";
export const USER_KEY = "als_user";

const http = axios.create({ baseURL: API_URL, timeout: 10000 });

// Attach the JWT to every request.
http.interceptors.request.use((config) => {
  const token = localStorage.getItem(TOKEN_KEY);
  if (token) config.headers.Authorization = `Bearer ${token}`;
  return config;
});

// Expired/invalid token on a normal request -> clear the session and go to /login.
// Login/register 401s (wrong password) are NOT session expiry, so they are skipped.
http.interceptors.response.use(
  (res) => res,
  (err) => {
    const url = err.config?.url || "";
    if (err.response?.status === 401 && !url.startsWith("/auth/")) {
      localStorage.removeItem(TOKEN_KEY);
      localStorage.removeItem(USER_KEY);
      if (window.location.pathname !== "/login") window.location.assign("/login");
    }
    return Promise.reject(err);
  }
);

// Human-readable message for any failed call.
export function errorMessage(err) {
  if (err.response?.data?.error) return err.response.data.error;
  if (err.request) return "Cannot reach the server. Is the backend running on port 8080?";
  return err.message || "Something went wrong";
}

// ---- auth ----
export const login = (username, password) =>
  http.post("/auth/login", { username, password }).then((r) => r.data);
export const register = (username, email, password) =>
  http.post("/auth/register", { username, email, password }).then((r) => r.data);

// ---- student ----
export const getWeakTopics = () => http.get("/analytics/weak-topics").then((r) => r.data.topics);
export const startLearning = () => http.post("/learning/start").then((r) => r.data);
export const submitAnswer = (questionId, optionId, timeTaken) =>
  http
    .post("/learning/submit-answer", {
      question_id: questionId,
      selected_option_id: optionId,
      time_taken: timeTaken,
    })
    .then((r) => r.data);

// ---- teacher ----
export const getStudents = () => http.get("/analytics/students").then((r) => r.data.students);

// ---- courses, study materials, question import ----
export const getCourses = () => http.get("/courses").then((r) => r.data.courses);
export const getCourse = (id) => http.get(`/courses/${id}`).then((r) => r.data); // {course, materials}
export const createCourse = (title, description) =>
  http.post("/courses", { title, description }).then((r) => r.data.course);
export const addMaterial = (courseId, title, body) =>
  http.post(`/courses/${courseId}/materials`, { title, body }).then((r) => r.data.material);
export const deleteMaterial = (materialId) => http.delete(`/materials/${materialId}`).then((r) => r.data);
export const importQuestions = (courseId, text) =>
  http.post(`/courses/${courseId}/questions/import`, { text }).then((r) => r.data.imported);
