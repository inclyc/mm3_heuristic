#include "Heuristic/ColorGraph.h"
#include "Heuristic/Graph.h"
#include <cassert>
#include <iostream>
#include <memory>
#include <set>
#include <string>
// 是否能正常退出
static void exit_test() {
  const std::string test_name("exit_test");
  std::cout << test_name << " ...\n";
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

  assert(p->connection());

  std::cout << test_name << " passed.\n";
}

static void random_tree() { auto r = Heuristic::random_bigraph(10, 9); }

int main() {
  exit_test();
  random_tree();
}
