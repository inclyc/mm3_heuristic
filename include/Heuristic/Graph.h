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
  size_t ecnt;

protected:
  std::unique_ptr<Edge[]> edges;
  std::unique_ptr<int[]> head;

public:
  Graph(int v, int e);
  void addEdge(int u, int v, float w);
};

} // namespace Heuristic

#endif
