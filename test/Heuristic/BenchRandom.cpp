#include "Heuristic/Graph.h"
#include <fmt/core.h>
#include <functional>
#include <utility>

int main() {
  for (int TestCase = 15; TestCase <= 100; TestCase++) {
    auto Average = [](int Case, int EdgeNum) -> std::pair<float, float> {
      float Sum1 = 0, Sum2 = 0;
      for (int I = 0; I < Case; I++) {
        auto G = Heuristic::randomBiGraph<Heuristic::MSTGraph>(15, EdgeNum);
        Sum1 += G->bruteForce().first;
        Sum2 += G->solve().first;
      }
      return std::make_pair(Sum1, Sum2);
    };
    auto [A, B] = Average(10, TestCase);
    fmt::print("{} {}\n", A, B);
  }
}
