// Test dynamic graph
// https://loj.ac/s/1527121

#include "DynamicGraph/Graph.h"
#include <cstdio>
#include <memory>
#include <vector>

int main() {
  int n, m;
  scanf("%d%d", &n, &m);
  auto dg = std::make_unique<DynamicGraph::Graph>(n);
  printf("%d", dg->getBlockSize(1));
}
