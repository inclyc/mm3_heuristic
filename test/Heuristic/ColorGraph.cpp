#include "Heuristic/ColorGraph.h"
#include "Heuristic/Graph.h"
#include <iostream>
#include <memory>
int main() {
  int n, m;
  std::cin >> n;
  std::cin >> m;
  auto p = std::make_unique<Heuristic::ColorGraph>(n);
  for (int i = 0; i < m; i++) {
    int u, v;
    float w;
    std::cin >> u;
    std::cin >> v;
    std::cin >> w;
    p->addBiEdge(u, v, w);
  }
  std::cout << p->solve() << std::endl;
}
