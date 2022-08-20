#include "DSU/DSU.h"
#include <cstddef>
#include <memory>

namespace DSU {
DSU::DSU(int VertexNum) {
  Fa = std::make_unique<int[]>(VertexNum + 1);
  for (int Vertex = 0; Vertex <= VertexNum; Vertex++) {
    Fa[Vertex] = Vertex;
  }
}

int DSU::findFa(int U) { return Fa[U] == U ? U : Fa[U] = findFa(Fa[U]); }
} // namespace DSU
