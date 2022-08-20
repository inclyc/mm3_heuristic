#include "Heuristic/ColorGraph.h"
#include "Heuristic/Graph.h"
#include <fmt/core.h>
#include <iostream>
#include <memory>
int main() {
  auto Graph = std::make_unique<Heuristic::MSTGraph>();
  std::cin >> *Graph;
  auto [Ans, AnsSet] = Graph->solve();
  std::cout << fmt::format("Max cut: {:.2f}", Ans) << std::endl;
  std::cout << fmt::format("Number of vertexes in answer set: {}",
                           AnsSet->size())
            << std::endl;
  for (const auto &U : *AnsSet) {
    std::cout << U << " ";
  }
  std::cout << std::endl;
}
