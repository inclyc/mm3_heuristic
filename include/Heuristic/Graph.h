#pragma once

#include <memory>
#include <vector>

namespace Heuristic {

struct Edge {
  int v;
  float w;

  bool operator<(const Edge &e) const { return w < e.w; }
};

class Graph {
protected:
  std::unique_ptr<std::vector<Edge>[]> edges;
  int vertexNum, edgeNum;

public:
  Graph(int v) {
    edges = std::make_unique<std::vector<Edge>[]>(v + 1);
    vertexNum = v;
  }
  int getVertexNum() const { return vertexNum; }
  void addEdge(int u, int v, float w);
  void addBiEdge(int u, int v, float w) {
    addEdge(u, v, w);
    addEdge(v, u, w);
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
std::unique_ptr<Graph> random_bigraph(int n_vertex, int n_edge);

} // namespace Heuristic