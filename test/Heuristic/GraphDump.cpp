#include "Heuristic/Graph.h"
#include <iostream>
int main() {
  for (int I = 0; I < 1000; I++) {
    auto G = Heuristic::randomBiGraph<Heuristic::Graph>(5, 15);
    std::cout << *G << std::endl;
  }
}
