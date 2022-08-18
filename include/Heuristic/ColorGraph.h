#pragma once
#include "Graph.h"
#include <memory>
#include <set>

namespace Heuristic {
class ColorGraph : public Graph {
private:
  std::unique_ptr<int[]> Color;
  bool checkSetConn(const std::set<int> &) const;
  std::set<int> ArticulationPoints;

public:
  ColorGraph(int V) : Graph(V) {
    // allocate (v + 1) std::vector
    // vertexes number might started from 1.
    Color = std::make_unique<int[]>(V + 1);
    ArticulationPoints = std::set<int>();
  }
  /// @brief give each vertex a color, stores in this->color
  /// each cut vertex should be colored '-1'
  /// @return the number of articulation points
  int getColor();

  /// @brief start from a small set \p S, perform Prim-like heuristic search
  /// @return approximate result
  std::pair<float, std::shared_ptr<std::set<int>>>
  solveSet(std::shared_ptr<std::set<int>> S);

  std::pair<float, std::shared_ptr<std::set<int>>> solveArticulation(int U);

  std::pair<float, std::shared_ptr<std::set<int>>> solve();

  void dumpArticulationPoints();

  void dumpColor();
};

} // namespace Heuristic
