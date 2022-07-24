#ifndef HEURISRIC_GRAPH_H
#define HEURISRIC_GRAPH_H

#include <memory>

namespace Heuristic {

struct Edge {
  int u, v, next;
  float w;
};

class Graph {
private:
  int vertexNum, edgeNum;

protected:
  std::unique_ptr<Edge[]> edges;
  std::unique_ptr<int[]> head;

public:
  Graph(int v, int e);
  int getVertexNum() const { return vertexNum; }
  void addEdge(int u, int v, float w);
  void addBiEdge(int u, int v, float w) {
    addEdge(u, v, w);
    addEdge(v, u, w);
  }
};

} // namespace Heuristic

#endif
