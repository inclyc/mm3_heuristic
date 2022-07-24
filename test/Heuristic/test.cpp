#include "Heuristic/ColorGraph.h"
#include <memory>
#include <set>

// 是否能正常退出
static void exit_test() {
  auto a = std::make_unique<std::set<int>>();
  auto b = std::make_unique<std::set<int>>();
  a->insert(1);
  b->insert(4);

  auto p = std::make_unique<Heuristic::ColorGraph>(5, 10, std::move(a),
                                                   std::move(b));

  p->addBiEdge(1, 2, 2);
  p->addBiEdge(2, 3, 3);
  p->addBiEdge(3, 4, 4);

  p->bfs();
}

int main() { exit_test(); }
