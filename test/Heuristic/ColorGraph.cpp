#include "Heuristic/ColorGraph.h"
#include "Heuristic/Graph.h"
#include <iostream>
#include <memory>

int main() {
  auto Graph = std::make_unique<Heuristic::ColorGraph>();
  std::cin >> *Graph;
  auto [Ans, AnsSet] = Graph->solve();
  Graph->dumpArticulationPoints();
  std::cout << Ans << std::endl;
  for (const auto &U : *AnsSet) {
    std::cout << U << " ";
  }
  std::cout << std::endl;
}
