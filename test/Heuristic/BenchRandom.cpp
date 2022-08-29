#include "Heuristic/Graph.h"
#include <fmt/core.h>
#include <functional>
#include <utility>

int main(int argc, char *argv[]) {
  int TestNum = 20;
  if (argc > 1) {
    TestNum = std::atoi(argv[1]);
  }
  fmt::print("Random rounds: {}\n", TestNum);
  for (int TestCase = 15; TestCase <= 100; TestCase++) {
    auto Average = [&](int Case,
                       int EdgeNum) -> std::tuple<float, float, float> {
      float Sum1 = 0, Sum2 = 0, Sum3 = 0;
      for (int I = 0; I < Case; I++) {
        auto G = Heuristic::randomBiGraph<Heuristic::MSTGraph>(15, EdgeNum);
        Sum1 += G->bruteForce().first;
        Sum2 += G->solve(1).first; // i.e. don't shuffle
        Sum3 += G->solve(TestNum).first;
      }
      return std::make_tuple(Sum1, Sum2, Sum3);
    };
    auto [A, B, C] = Average(10, TestCase);
    fmt::print("{} {} {}\n", A, B, C);
  }
}
