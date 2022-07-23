#include "DynamicGraph/Graph.hpp"
#include "DynamicGraph/LinkCutTree.hpp"
#include <algorithm>
#include <cstdio>
#include <vector>

#define l(u) ch[u][0]
#define r(u) ch[u][1]

namespace DynamicGraph {

static const int N = 5e5 + 10;
ll Graph::mapNode(int x, int y) { return (ll)x * M + y; }
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
  lct[lv].access(u), lct[lv].access(v);
  if (lct[lv].siz[u] > lct[lv].siz[v])
    std::swap(u, v);
  int t = u, replacev = 0;
  while ((u = lct[lv].get(u, 0))) {
    std::unordered_set<int> &G = lct[lv].G[u][0];
    while (G.size()) {
      int v = *G.begin();
      deleteTree(lv, u, v), insertAsTree(lv + 1, u, v), lct[lv + 1].link(u, v);
    }
  }
  u = t;
  while ((u = lct[lv].get(u, 1))) {
    std::unordered_set<int> &G = lct[lv].G[u][1];
    while (G.size()) {
      int v = *G.begin();
      if (lct[lv].isconnected(u, v))
        deleteGraph(lv, u, v), insertAsGraph(lv + 1, u, v);
      else {
        replacev = v, deleteGraph(lv, u, replacev),
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
  if (!isConnected(u, v))
    lct[0].link(u, v), insertAsTree(0, u, v), --blocks;
  else
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
  for (int i = 0; i < 20; ++i)
    lct[i].init(n);
  LV.clear(), blocks = n;
  N = n;
  M = 1e9 + 7;
}

int Graph::getBlock() { return blocks; }
} // namespace DynamicGraph