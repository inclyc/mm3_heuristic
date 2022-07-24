#ifndef HEURISTIC_COLOR_GRAPH_H
#define HEURISTIC_COLOR_GRAPH_H

#include "Graph.h"
#include <memory>
#include <set>

namespace Heuristic {
class ColorGraph : public Graph {
private:
  std::set<int> A, B;
  bool checkSetConn(const std::set<int> &) const;

public:
  ColorGraph(int v, int e, std::set<int> &&A_, std::set<int> &&B_)
      : Graph(v, e) {
    A = std::move(A_);
    B = std::move(B_);
  }

  ColorGraph(Graph &&g, std::set<int> A_, std::set<int> B_)
      : Graph(std::move(g)) {
    A = std::move(A_);
    B = std::move(B_);
  }

  void bfs();

  // 连通性检查
  bool connection() const { return checkSetConn(A) && checkSetConn(B); }

  // 所有的节点都已经染色完成
  bool allNodesColored() const {
    return static_cast<size_t>(getVertexNum()) == A.size() + B.size();
  }

  // 所有的检查
  bool validate() const { return connection() && allNodesColored(); }

  // 割，A, B集合的边权和
  float cut() const;
};

} // namespace Heuristic

#endif
