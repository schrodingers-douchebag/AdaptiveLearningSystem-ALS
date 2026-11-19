import { useMemo } from "react";
import { knapsack } from "../utils/knapsack.js";

// re-runs the DP only when the items or the time limit change
export default function useKnapsack(items, capacity) {
  return useMemo(() => knapsack(items, capacity), [items, capacity]);
}
