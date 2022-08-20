#include "Heuristic/Graph.h"
#include <fmt/core.h>

int main() {
  for (int TestCase = 0; TestCase < 100; TestCase++) {
    auto G = Heuristic::randomBiGraph<Heuristic::MSTGraph>(15, 30);
    fmt::print("{} {}\n", G->bruteForce().first, G->solve().first);
  }
}
