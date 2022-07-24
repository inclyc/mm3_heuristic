#ifndef HEURISTIC_COLOR_GRAPH_H
#define HEURISTIC_COLOR_GRAPH_H

#include "Graph.h"
#include <set>

namespace Heuristic {
class ColorGraph : public Graph {
  std::unique_ptr<std::set<int>> A, B;

public:
  ColorGraph(int v, int e, std::unique_ptr<std::set<int>> A_,
             std::unique_ptr<std::set<int>> B_)
      : Graph(v, e) {
    A = std::move(A_);
    B = std::move(B_);
  }

  void bfs();
};

} // namespace Heuristic

#endif
