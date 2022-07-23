#include "DynamicGraph/Graph.h"
#include "DynamicGraph/LinkCutTree.h"
#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <vector>

#define l(u) ch[u][0]
#define r(u) ch[u][1]

namespace DynamicGraph {

ll Graph::mapNode(int x, int y) { return static_cast<ll>(x) * M + y; }
void Graph::insertAsTree(int lv, int u, int v) {
  LV[mapNode(u, v)] = LV[mapNode(v, u)] = lv;
  lct[lv].ins(0, u, v);
}
void Graph::insertAsGraph(int lv, int u, int v) {
  LV[mapNode(u, v)] = LV[mapNode(v, u)] = lv;
  lct[lv].ins(1, u, v);
}
void Graph::deleteTree(int lv, int u, int v) { lct[lv].del(0, u, v); }
void Graph::deleteGraph(int lv, int u, int v) { lct[lv].del(1, u, v); }
bool Graph::findReplace(int lv, int u, int v) {
  lct[lv].access(u);
  lct[lv].access(v);
  if (lct[lv].siz[u] > lct[lv].siz[v])
    std::swap(u, v);
  int t = u, replacev = 0;
  while ((u = lct[lv].get(u, 0))) {
    std::unordered_set<int> &G = lct[lv].G[u][0];
    while (G.size()) {
      int nxt = *G.begin();
      deleteTree(lv, u, nxt);
      insertAsTree(lv + 1, u, nxt);
      lct[lv + 1].link(u, nxt);
    }
  }
  u = t;
  while ((u = lct[lv].get(u, 1))) {
    std::unordered_set<int> &G = lct[lv].G[u][1];
    while (G.size()) {
      int nxt = *G.begin();
      if (lct[lv].isconnected(u, nxt)) {
        deleteGraph(lv, u, nxt);
        insertAsGraph(lv + 1, u, nxt);
      } else {
        replacev = nxt;
        deleteGraph(lv, u, replacev);
        insertAsTree(lv, u, replacev);
        for (int i = 0; i <= lv; ++i)
          lct[i].link(u, replacev);
        break;
      }
    }
    if (replacev)
      return 1;
  }
  return 0;
}
int Graph::isConnected(int u, int v) { return lct[0].isconnected(u, v); }
int Graph::getBlockSize(int u) {
  lct[0].access(u);
  return lct[0].siz[u];
}
void Graph::link(int u, int v) {
  if (!isConnected(u, v)) {
    lct[0].link(u, v);
    insertAsTree(0, u, v);
    --blocks;
  } else
    insertAsGraph(0, u, v);
}
void Graph::cut(int u, int v) {
  int lv = LV[mapNode(u, v)];
  if (lct[lv].G[u][0].count(v)) {
    for (int i = 0; i <= lv; ++i)
      lct[i].cut(u, v);
    deleteTree(lv, u, v);
    ++blocks;
    for (int i = lv; i >= 0; --i)
      if (findReplace(i, u, v)) {
        --blocks;
        break;
      }
  } else
    deleteGraph(lv, u, v);
}
Graph::~Graph() {
  for (int i = 0; i < 20; ++i)
    lct[i].free();
}

Graph::Graph(int n) {
  for (std::size_t i = 0; i < 20; ++i)
    lct[i].init(static_cast<std::size_t>(n));
  LV.clear();
  blocks = n;
  N = n;
  M = 1000000000 + 7;
}

int Graph::getBlock() { return blocks; }
} // namespace DynamicGraph
