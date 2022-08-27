#include "Heuristic/Graph.h"
#include <cstdlib>
#include <fmt/core.h>
#include <iostream>
#include <memory>
#include <sstream>
int main(int argc, char *argv[]) {
  int TestNum = 20;
  if (argc > 1) {
    TestNum = std::atoi(argv[1]);
  }
  fmt::print("Random rounds: {}\n", TestNum);
  auto Graph = std::make_unique<Heuristic::MSTGraph>();
  std::cin >> *Graph;
  auto [Ans, AnsSet] = Graph->solve(TestNum);
  fmt::print("Max cut: {:.2f}\n", Ans);
  fmt::print("Number of vertexes in the answer set: {}\n", AnsSet->size());
  Graph->printSetByEdge(*AnsSet);
}
