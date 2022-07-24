#ifndef HEURISTIC_COLOR_GRAPH_H
#define HEURISTIC_COLOR_GRAPH_H

#include "Graph.h"
#include <set>

namespace Heuristic {
class ColorGraph : public Graph {
private:
  std::unique_ptr<std::set<int>> A, B;
  bool checkSetConn(const std::set<int> &) const;

public:
  ColorGraph(int v, int e, std::unique_ptr<std::set<int>> A_,
             std::unique_ptr<std::set<int>> B_)
      : Graph(v, e) {
    A = std::move(A_);
    B = std::move(B_);
  }

  void bfs();

  // 连通性检查
  bool connection() const { return checkSetConn(*A) && checkSetConn(*B); }

  // 所有的节点都已经染色完成
  bool allNodesColored() const;

  // 所有的检查
  bool validate() const { return connection() && allNodesColored(); }

  // 割，A, B集合的边权和
  float cut() const;
};

} // namespace Heuristic

#endif
