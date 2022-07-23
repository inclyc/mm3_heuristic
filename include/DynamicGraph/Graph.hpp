// Author: lyc
// 2021/10/31
// 利用LCT维护生成树
#ifndef DYNAMIC_GRAPH_HPP
#define DYNAMIC_GRAPH_HPP
#include "LinkCutTree.hpp"
#include <cstddef>
#include <unordered_map>
namespace DynamicGraph {
typedef long long ll;
class Graph {

private:
  int N;
  int M;
  int blocks;
  ll mapNode(int x, int y);
  LinkCutTree lct[20];
  std::unordered_map<ll, int> LV;
  void insertAsTree(int lv, int u, int v);
  void insertAsGraph(int lv, int u, int v);
  void deleteTree(int lv, int u, int v);
  void deleteGraph(int lv, int u, int v);
  bool findReplace(int lv, int u, int v);

public:
  int getBlockSize(int u);
  int getBlock();
  Graph(int);
  ~Graph();
  int isConnected(int u, int v);
  void link(int u, int v);
  void cut(int u, int v);
};
} // namespace DynamicGraph

#endif // DYNAMIC_GRAPH_HPP
