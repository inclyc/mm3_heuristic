#include "Heuristic/ColorGraph.h"
#include "Heuristic/Graph.h"
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <set>
#include <string>
// 是否能正常退出
static void exit_test() {
  const std::string test_name("exit_test");
  std::cout << test_name << " ...\n";
  auto a = std::set<int>();
  auto b = std::set<int>();
  a.insert(1);
  b.insert(4);

  auto p = std::make_unique<Heuristic::ColorGraph>(5, 10, std::move(a),
                                                   std::move(b));

  p->addBiEdge(1, 2, 2);
  p->addBiEdge(2, 3, 3);
  p->addBiEdge(3, 4, 4);

  p->bfs();

  assert(p->connection());

  std::cout << test_name << " passed.\n";
}

static void random_tree_test() {
  const std::string test_name("random_tree_test");
  std::cout << test_name << " ...\n";

  for (int i = 0; i < 1000; i++) {
    int n = rand() % 1000 + 20;
    auto g = Heuristic::random_bigraph(n, n - 1);
    auto a = std::set<int>();
    auto b = std::set<int>();
    a.insert(1);
    b.insert(4);
    auto hcg = std::make_unique<Heuristic::ColorGraph>(
        std::move(*g), std::move(a), std::move(b));
    hcg->bfs();
    assert(hcg->validate());
  }

  std::cout << test_name << " passed.\n";
}

int main() {
  exit_test();
  random_tree_test();
}
