#pragma once

#include <memory>
namespace Heuristic {
class DisjointSet {
private:
  std::unique_ptr<int[]> Fa;
  int findFa(int);

public:
  explicit DisjointSet(int VertexNum);
  void link(int U, int V) { Fa[static_cast<size_t>(findFa(U))] = findFa(V); }
  bool isConnected(int U, int V) { return findFa(U) == findFa(V); }
};

}; // namespace Heuristic
