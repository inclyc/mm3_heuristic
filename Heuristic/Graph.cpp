#include "Heuristic/Graph.h"
namespace Heuristic {
void Graph::addEdge(int u, int v, float w) {
  edgeNum++;
  auto &hdu = head[static_cast<size_t>(u)];
  edges[static_cast<size_t>(edgeNum)] =
      Edge{.u = u, .v = v, .next = hdu, .w = w};
  hdu = edgeNum;
}

Graph::Graph(int v, int e) {
  edges = std::make_unique<Edge[]>(static_cast<size_t>(e + 1));
  head = std::make_unique<int[]>(static_cast<size_t>(v + 1));
  edgeNum = 0;
  vertexNum = v;
}

} // namespace Heuristic
