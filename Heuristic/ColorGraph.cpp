#include "Heuristic/ColorGraph.h"
#include "Heuristic/Graph.h"
#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <queue>
#include <vector>
namespace Heuristic {
int ColorGraph::getColor() {
  auto low = std::make_unique<int[]>(vertexNum + 1);
  auto dfn = std::make_unique<int[]>(vertexNum + 1);
  auto vis = std::make_unique<bool[]>(vertexNum + 1);
  articulation_points.clear();
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
          articulation_points.insert(u);
          color[u] = -1;
        }
      } else if (v != fa) {
        low[u] = std::min(low[u], dfn[v]);
      }
    }
    if (u == fa && child > 1) {
      articulation_points.insert(u);
      color[u] = -1;
    }
  };
  // by Robert E. Tarjan
  // O(n)
  for (int i = 1; i <= vertexNum; i++) {
    if (!vis[i]) {
      tarjan(i, i);
    }
  }
  // Assign each node in BCC an ID and stores in 'color'
  // the color of some vertexes u is '-1', indicates that
  // these vertexes are articulation points
  cnt = 0; // for allocating ID
  std::function<void(int)> dfs;

  // give node u as color `cnt`, and walk through dfs tree recursively
  // don't step in articulation or colored points.
  dfs = [&](int u) {
    color[u] = cnt;
    for (const auto &e : edges[u]) {
      int v = e.v;
      if (!color[v]) {
        // i.e. we haven't assigned a color to v
        dfs(v);
      }
    }
  };

  for (int i = 1; i <= vertexNum; i++) {
    if (!color[i]) {
      ++cnt;
      dfs(i);
    }
  }
  return articulation_points.size();
}

float ColorGraph::solveSet(std::set<int> s) {
  float tmp = 0.0f, ans = -1e5;
  std::priority_queue<Edge> pq;
  for (const auto &u : s) {
    for (const auto &e : edges[u]) {
      int v = e.v;
      float w = e.w;
      if (!s.contains(v) && color[v] == color[u]) {
        pq.push(e);
        tmp += w;
      }
    }
  }
  while (!pq.empty()) {
    ans = std::max(ans, tmp);
    const auto &tp = pq.top();
    pq.pop();
    int u = tp.v;
    if (s.contains(u))
      continue;
    s.insert(u);
    for (const auto &e : edges[u]) {
      if (color[e.v] != color[u] || e.v == u)
        continue;
      if (s.contains(e.v)) {
        tmp -= e.w;
      } else {
        tmp += e.w;
        pq.push(e);
      }
    }
  }
  return ans;
}

float ColorGraph::solveArticulation(int u) {
  float ans = -1e5f;
  std::map<int, std::set<int>> mp;
  std::map<int, float> art_cut;
  mp.clear();
  for (const auto &e : edges[u]) {
    if (e.v == u)
      continue; // self loops
    if (!mp.contains(color[e.v])) {
      mp.insert({color[e.v], std::set<int>()});
      art_cut.insert({color[e.v], 0});
    }
    mp[color[e.v]].insert(e.v);
    art_cut[color[e.v]] += e.w;
  }
  for (const auto &k : art_cut) {
    ans = std::max(ans, k.second);
  }
  for (const auto &s : mp) {
    ans = std::max(ans, solveSet(s.second));
  }
  return ans;
}

float ColorGraph::solve() {
  float ans = -1e5f;
  getColor();
  for (const auto &u : articulation_points) {
    ans = std::max(ans, solveArticulation(u));
  }
  return ans;
}

} // namespace Heuristic
