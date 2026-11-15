#include "BellmanFord.h"
#include <limits>

BellmanFordResult bellmanFord(int n, const std::vector<WeightedEdge>& edges, int source) {
    const int INF = std::numeric_limits<int>::max() / 4;
    if (n <= 0 || source < 0 || source >= n) return {false, false, {}};
    std::vector<int> dist(n, INF);
    dist[source] = 0;
    for (int i = 1; i < n; ++i) {
        bool changed = false;
        for (const auto& e : edges) {
            if (e.from < 0 || e.from >= n || e.to < 0 || e.to >= n) continue;
            if (dist[e.from] != INF && dist[e.from] + e.weight < dist[e.to]) {
                dist[e.to] = dist[e.from] + e.weight;
                changed = true;
            }
        }
        if (!changed) break;
    }
    bool negative = false;
    for (const auto& e : edges) {
        if (e.from >= 0 && e.from < n && e.to >= 0 && e.to < n &&
            dist[e.from] != INF && dist[e.from] + e.weight < dist[e.to]) {
            negative = true; break;
        }
    }
    return {dist[source] != INF, negative, dist};
}
