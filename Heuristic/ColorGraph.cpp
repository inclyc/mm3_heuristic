#include "Heuristic/ColorGraph.h"
#include "Heuristic/Graph.h"
#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <vector>

void Heuristic::ColorGraph::getColor() {
  auto low = std::make_unique<int[]>(vertexNum + 1);
  auto dfn = std::make_unique<int[]>(vertexNum + 1);
  auto vis = std::make_unique<bool[]>(vertexNum + 1);
  auto ans = std::set<int>();
  for (int i = 0; i < vertexNum + 1; i++) {
    low[i] = dfn[i] = 0;
    vis[i] = false;
  }
  int cnt = 1;
  std::function<void(int, int)> tarjan;
  tarjan = [&](int u, int fa) {
    vis[u] = true;
    low[u] = dfn[u] = cnt++;
    int child = 0;
    for (const auto &e : edges[u]) {
      int v = e.v;
      if (!vis[v]) {
        child++;
        tarjan(v, u);
        low[u] = std::min(low[u], low[v]);
        if (fa != u && low[v] >= dfn[u]) {
          ans.insert(u);
        }
      } else if (v != fa) {
        low[u] = std::min(low[u], dfn[v]);
      }
    }
    if (u == fa && child > 1)
      ans.insert(u);
  };
  // by Robert E. Tarjan
  // O(n)
  for (int i = 1; i <= vertexNum; i++) {
    if (!vis[i]) {
      tarjan(i, i);
    }
  }
}