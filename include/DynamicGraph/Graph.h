// Author: lyc
// 2021/10/31
// 利用LCT维护生成树
#ifndef DYNAMIC_GRAPH_H
#define DYNAMIC_GRAPH_H
#include "LinkCutTree.h"
#include <cstddef>
#include <unordered_map>
namespace DynamicGraph {
typedef long long ll;
class Graph {

private:
  int N;
  int M;
  int Blocks;
  ll mapNode(int X, int Y);
  LinkCutTree LCT[20];
  std::unordered_map<ll, int> LV;
  void insertAsTree(int Level, int U, int V);
  void insertAsGraph(int Level, int U, int V);
  void deleteTree(int Level, int U, int V);
  void deleteGraph(int Level, int U, int V);
  bool findReplace(int Level, int U, int V);

public:
  int getBlockSize(int U);
  int getBlock();
  Graph(int);
  ~Graph();
  int isConnected(int U, int V);
  void link(int U, int V);
  void cut(int U, int V);
};
} // namespace DynamicGraph

#endif // DYNAMIC_GRAPH_H
