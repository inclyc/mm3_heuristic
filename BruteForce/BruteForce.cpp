#include "Heuristic/Graph.h"
#include <iostream>
#include <memory>
int main() {
  auto Graph = std::make_unique<Heuristic::Graph>();
  std::cin >> *Graph;
}
