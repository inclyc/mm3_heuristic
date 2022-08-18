#pragma once

#include <memory>
#include <vector>

namespace Heuristic {

struct Edge {
  int V;
  float W;

  bool operator<(const Edge &E) const { return W < E.W; }
};

class Graph {
protected:
  std::unique_ptr<std::vector<Edge>[]> Edges;
  int VertexNum, EdgeNum;

public:
  Graph(int V) {
    Edges = std::make_unique<std::vector<Edge>[]>(V + 1);
    VertexNum = V;
  }
  int getVertexNum() const { return VertexNum; }
  void addEdge(int U, int V, float W);
  void addBiEdge(int U, int V, float W) {
    addEdge(U, V, W);
    addEdge(V, U, W);
  }
};

/**
 * @brief generates a random bi-connected graph
 *
 * @param n_vertex number of vertex
 * @param n_edge number of edge
 * @note ensure e >= v - 1
 * @return std::unique_ptr<Graph>
 */
std::unique_ptr<Graph> randomBiGraph(int VertexNum, int EdgeNum);

} // namespace Heuristic
