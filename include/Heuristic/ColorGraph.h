#pragma once
#include "Graph.h"
#include <memory>
#include <set>

namespace Heuristic {
class ColorGraph : public Graph {
private:
  std::unique_ptr<int[]> color;
  bool checkSetConn(const std::set<int> &) const;
  std::set<int> articulation_points;

public:
  ColorGraph(int v) : Graph(v) {
    // allocate (v + 1) std::vector
    // vertexes number might started from 1.
    color = std::make_unique<int[]>(v + 1);
    articulation_points = std::set<int>();
  }
  /// @brief give each vertex a color, stores in this->color
  /// each cut vertex should be colored '-1'
  /// @return the number of articulation points
  int getColor();

  /// @brief start from a small set s, perform Prim-like heuristic search
  /// @return approximate result
  float solveSet(std::set<int> s);

  float solveArticulation(int u);

  float solve();
};

} // namespace Heuristic
