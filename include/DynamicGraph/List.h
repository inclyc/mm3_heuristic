#pragma once
#include <memory>
namespace DynamicGraph {
struct List {
  std::unique_ptr<int[]> Head;
  std::unique_ptr<int[]> Precessor;
  std::unique_ptr<int[]> Successor;
  void init(unsigned long);
  void link(int, int);
  void cut(int, int);
  bool empty(int);
  int first(int);
};
} // namespace DynamicGraph
