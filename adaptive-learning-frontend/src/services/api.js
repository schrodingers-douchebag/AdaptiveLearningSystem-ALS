import { students, topics, activities } from "./mockData.js";

// Demo service: fake network delay + mock data.
// Later, swap these bodies with fetch(`${API_URL}/...`).
export const API_URL = import.meta.env.VITE_API_URL;
const wait = (ms = 250) => new Promise((r) => setTimeout(r, ms));

export async function getStudents() { await wait(); return students; }
export async function getTopics() { await wait(); return topics; }
export async function getActivities() { await wait(); return activities; }
