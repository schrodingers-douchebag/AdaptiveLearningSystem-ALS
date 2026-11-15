#pragma once
#include <vector>
struct WeightedEdge {
    int from{};
    int to{};
    int weight{};
};
struct BellmanFordResult {
    bool reachable{};
    bool hasNegativeCycle{};
    std::vector<int> distances;
};
BellmanFordResult bellmanFord(int vertexCount, const std::vector<WeightedEdge>& edges, int source);
