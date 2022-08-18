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
  ColorGraph(int VertexNum) : Graph(VertexNum) {
    // allocate (v + 1) std::vector
    // vertexes number might started from 1.
    ColorGraph();
    setVertexNum(VertexNum);
  }

  ColorGraph() { ArticulationPoints = std::set<int>(); }

  void setVertexNum(int VertexNum);

  /// @brief give each vertex a color, stores in this->color
  /// each cut vertex should be colored '-1'
  /// @return the number of articulation points
  int getArticulationPoints();

  /// @brief start from a small set \p S, perform Prim-like heuristic search
  /// @return approximate result
  std::pair<float, std::shared_ptr<std::set<int>>>
  solveSet(std::shared_ptr<std::set<int>> S, int WorkColor);

  std::pair<float, std::shared_ptr<std::set<int>>> solveArticulation(int U);

  std::pair<float, std::shared_ptr<std::set<int>>> solve();

  void colorArticulationPoint(int A);

  void dumpArticulationPoints();

  void dumpColor();
};

} // namespace Heuristic
