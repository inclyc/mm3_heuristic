#include "Heuristic/Graph.h"
#include <iostream>

int main() {
  auto G = Heuristic::randomBiGraph<Heuristic::Graph>(15, 20);
  std::cout << *G << std::endl;
}
