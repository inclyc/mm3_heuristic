#include "Heuristic/Graph.h"
#include <fmt/core.h>
#include <functional>
#include <utility>

int main(int argc, char *argv[]) {
  int TestNum = 20;
  if (argc > 1) {
    TestNum = std::atoi(argv[1]);
  }
  int EdgeNumBegin = 15;
  int EdgeNumEnd = 80;
  int VertexNum = 15;
  int CaseNum = 20;
  assert(VertexNum - 1 <= EdgeNumBegin);
  assert(VertexNum * (VertexNum - 1) / 2 <= EdgeNumEnd);
  for (int EdgeNum = EdgeNumBegin; EdgeNum != EdgeNumEnd; EdgeNum++) {
    fmt::print(stderr, "{}\r", EdgeNum);
    auto Average = [&](int Case,
                       int EdgeNum) -> std::tuple<float, float, float> {
      float Sum1 = 0, Sum2 = 0, Sum3 = 0;
      for (int I = 0; I < Case; I++) {
        auto G =
            Heuristic::randomBiGraph<Heuristic::MSTGraph>(VertexNum, EdgeNum);
        Sum1 += G->bruteForce().first;
        Sum2 += G->solve(1).first; // i.e. don't shuffle
        Sum3 += G->solve(TestNum).first;
      }
      return std::make_tuple(Sum1 / Case, Sum2 / Case, Sum3 / Case);
    };
    auto [A, B, C] = Average(CaseNum, EdgeNum);
    fmt::print("{} {} {}\n", A, B, C);
  }
}
