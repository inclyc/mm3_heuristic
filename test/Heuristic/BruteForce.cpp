#include "Heuristic/Graph.h"
#include <fmt/core.h>
#include <iostream>
#include <memory>

int main() {
  auto Graph = std::make_unique<Heuristic::MSTGraph>();
  std::cin >> *Graph;
  auto [Ans, AnsSet] = Graph->bruteForce();
  fmt::print("Max cut: {:.2f}\n", Ans);
  fmt::print("Number of vertexes in the answer set: {}\n", AnsSet->size());
  Graph->printSetByEdge(*AnsSet);
}
