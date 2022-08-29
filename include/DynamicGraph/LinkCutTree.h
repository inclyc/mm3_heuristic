// Author: lyc
// 2021/10/31
// Link-Cut-Trees by Robert Endre Tarjan
// 为其他结构提供动态树支持。
// 在这个项目中，LCT被用于： 维护动态图中的生成树
#pragma once
#include "List.h"
#include "unordered_set"
#include <algorithm>
#include <cstddef>
#include <memory>
#define LCT_MAXN 5000
namespace DynamicGraph {
struct LinkCutTree {
  std::unordered_set<int> G[LCT_MAXN][2];
  int ChildVertex[LCT_MAXN][2];
  int Tag[LCT_MAXN][2];

  std::unique_ptr<int[]> FatherVertex;
  std::unique_ptr<int[]> FlippedFlag;
  std::unique_ptr<int[]> Stack;
  std::unique_ptr<int[]> Size;
  std::unique_ptr<int[]> SizeValue;
  List ChildV[2];

  void init(int VertexNum);
  void linkv(int, int);
  void cutv(int, int);
  void rev(int);
  void pu(int);
  void pd(int);
  int sf(int);
  bool isrt(int);
  void rot(int);
  void splay(int);
  void access(int);
  void makert(int);
  void join(int, int);
  int findrt(int);
  void link(int, int);
  void cut(int, int);
  int get(int, int);
  bool isconnected(int, int);
  void ins(int, int, int);
  void del(int, int, int);
};
} // namespace DynamicGraph
