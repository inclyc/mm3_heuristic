#include "Random/Random.h"
#include <cassert>
#include <cstdlib>

int randRange(int L, int R) { return rand() % (R - L) + L; }

float randFloat() {
  return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

std::set<int> randSet(int N, int L, int R) {
  assert(R >= L && R >= L + N);
  std::set<int> Ans;
  while (Ans.size() < static_cast<size_t>(N)) {
    int Ele = randRange(L, R);
    Ans.insert(Ele);
  }
  return Ans;
}
