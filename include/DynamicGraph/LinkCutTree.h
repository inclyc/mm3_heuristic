// Author: lyc
// 2021/10/31
// Link-Cut-Trees by Robert Endre Tarjan
// 为其他结构提供动态树支持。
// 在这个项目中，LCT被用于： 维护动态图中的生成树
#ifndef LINK_CUT_TREE_HPP
#define LINK_CUT_TREE_HPP

#include "List.h"
#include "unordered_set"
#include <algorithm>
#include <cstddef>

#define LCT_MAXN 100000 + 10
namespace DynamicGraph {
struct LinkCutTree {
  std::unordered_set<int> G[LCT_MAXN][2];
  int fa[LCT_MAXN];
  int ch[LCT_MAXN][2];
  int flp[LCT_MAXN];
  int sta[LCT_MAXN];
  int tp;
  int siz[LCT_MAXN];
  int sizv[LCT_MAXN];
  int tag[LCT_MAXN][2];

  List chv[2];

  void init(std::size_t);
  void free();

  void linkv(int, int);
  void cutv(int u, int v);
  void rev(int u);
  void pu(int u);
  void pd(int u);
  int sf(int u);
  bool isrt(int u);
  void rot(int u);
  void splay(int u);
  void access(int u);
  void makert(int u);
  void join(int u, int v);
  int findrt(int u);
  void link(int u, int v);
  void cut(int u, int v);
  int get(int u, int f);
  bool isconnected(int u, int v);
  void ins(int f, int u, int v);
  void del(int f, int u, int v);
};
} // namespace DynamicGraph

#endif // LINK_CUT_TREE_HPP
