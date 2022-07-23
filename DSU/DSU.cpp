#include "DSU/DSU.h"
#include <cstddef>
#include <memory>

namespace DSU {
DSU::DSU(int n) {
  fa = std::make_unique<int[]>(static_cast<size_t>(n));
  for (int i = 1; i <= n; i++) {
    fa[static_cast<size_t>(i)] = i;
  }
}

int DSU::findFa(int u) {
  int &f = fa[static_cast<size_t>(u)];
  if (f == u) {
    return f;
  } else {
    f = findFa(f);
    return f;
  }
}
} // namespace DSU
