#include "DynamicGraph/Graph.h"
#include "DynamicGraph/LinkCutTree.h"
#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <memory>
#include <vector>

#define l(u) ch[u][0]
#define r(u) ch[u][1]

namespace DynamicGraph {

ll Graph::mapNode(int X, int Y) { return static_cast<ll>(X) * M + Y; }
void Graph::insertAsTree(int Level, int U, int V) {
  LV[mapNode(U, V)] = LV[mapNode(V, U)] = Level;
  LCT[Level].ins(0, U, V);
}
void Graph::insertAsGraph(int Level, int U, int V) {
  LV[mapNode(U, V)] = LV[mapNode(V, U)] = Level;
  LCT[Level].ins(1, U, V);
}
void Graph::deleteTree(int Level, int U, int V) { LCT[Level].del(0, U, V); }
void Graph::deleteGraph(int Level, int U, int V) { LCT[Level].del(1, U, V); }
bool Graph::findReplace(int Level, int U, int V) {
  LCT[Level].access(U);
  LCT[Level].access(V);
  if (LCT[Level].Size[U] > LCT[Level].Size[V])
    std::swap(U, V);
  int T = U, ReplaceV = 0;
  while ((U = LCT[Level].get(U, 0))) {
    std::unordered_set<int> &G = LCT[Level].G[U][0];
    while (G.size()) {
      int Next = *G.begin();
      deleteTree(Level, U, Next);
      insertAsTree(Level + 1, U, Next);
      LCT[Level + 1].link(U, Next);
    }
  }
  U = T;
  while ((U = LCT[Level].get(U, 1))) {
    std::unordered_set<int> &G = LCT[Level].G[U][1];
    while (G.size()) {
      int Next = *G.begin();
      if (LCT[Level].isconnected(U, Next)) {
        deleteGraph(Level, U, Next);
        insertAsGraph(Level + 1, U, Next);
      } else {
        ReplaceV = Next;
        deleteGraph(Level, U, ReplaceV);
        insertAsTree(Level, U, ReplaceV);
        for (int I = 0; I <= Level; ++I)
          LCT[I].link(U, ReplaceV);
        break;
      }
    }
    if (ReplaceV)
      return 1;
  }
  return 0;
}
int Graph::isConnected(int U, int V) { return LCT[0].isconnected(U, V); }
int Graph::getBlockSize(int U) {
  LCT[0].access(U);
  return LCT[0].Size[U];
}
void Graph::link(int U, int V) {
  if (!isConnected(U, V)) {
    LCT[0].link(U, V);
    insertAsTree(0, U, V);
    --Blocks;
  } else
    insertAsGraph(0, U, V);
}
void Graph::cut(int U, int V) {
  int Level = LV[mapNode(U, V)];
  if (LCT[Level].G[U][0].count(V)) {
    for (int I = 0; I <= Level; ++I)
      LCT[I].cut(U, V);
    deleteTree(Level, U, V);
    ++Blocks;
    for (int LevelIndex = Level; LevelIndex >= 0; --LevelIndex)
      if (findReplace(LevelIndex, U, V)) {
        --Blocks;
        break;
      }
  } else
    deleteGraph(Level, U, V);
}

Graph::Graph(int VertexNum) {
  LCT = std::make_unique<LinkCutTree[]>(20);
  for (std::size_t I = 0; I < 20; ++I) {
    LCT[I].init(VertexNum);
  }

  LV.clear();
  Blocks = VertexNum;
  N = VertexNum;
  M = 1000000000 + 7;
}

int Graph::getBlock() { return Blocks; }
} // namespace DynamicGraph
