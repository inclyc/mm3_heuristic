#include "Heuristic/DisjointSet.h"
#include <cstddef>
#include <memory>

namespace Heuristic {
DisjointSet::DisjointSet(int VertexNum) {
  Fa = std::make_unique<int[]>(VertexNum + 1);
  for (int Vertex = 0; Vertex <= VertexNum; Vertex++) {
    Fa[Vertex] = Vertex;
  }
}

int DisjointSet::findFa(int U) {
  return Fa[U] == U ? U : Fa[U] = findFa(Fa[U]);
}
} // namespace Heuristic
