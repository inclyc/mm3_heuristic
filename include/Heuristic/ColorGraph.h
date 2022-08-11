#pragma once
#include "Graph.h"
#include <memory>
#include <set>

namespace Heuristic {
class ColorGraph : public Graph {
private:
  std::unique_ptr<int[]> color;
  bool checkSetConn(const std::set<int> &) const;

public:
  ColorGraph(int v) : Graph(v) {
    // allocate (v + 1) std::vector
    // vertexes number might started from 1.
    color = std::make_unique<int[]>(v + 1);
  }
  // give each vertex a color, stores in this->color
  // each cut vertex should be colored '-1'
  int getColor();
};

} // namespace Heuristic
