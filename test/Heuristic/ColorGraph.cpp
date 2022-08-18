#include "Heuristic/ColorGraph.h"
#include "Heuristic/Graph.h"
#include <iostream>
#include <memory>

int main() {
  int VertexNum, EdgeNum;
  std::cin >> VertexNum;
  std::cin >> EdgeNum;
  auto Graph = std::make_unique<Heuristic::ColorGraph>(VertexNum);
  for (int I = 0; I < EdgeNum; I++) {
    int U, V;
    float W;
    std::cin >> U;
    std::cin >> V;
    std::cin >> W;
    Graph->addBiEdge(U, V, W);
  }
  auto [Ans, AnsSet] = Graph->solve();
  Graph->dumpArticulationPoints();
  std::cout << Ans << std::endl;
  for (const auto &U : *AnsSet) {
    std::cout << U << " ";
  }
  std::cout << std::endl;
}
