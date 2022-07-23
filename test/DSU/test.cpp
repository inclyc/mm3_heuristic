#include "DSU/DSU.h"
#include <cassert>
#include <memory>

int main() {
  auto dsu = std::make_unique<DSU::DSU>(10);
  dsu->link(1, 2);
  assert(dsu->isConnected(1, 2));
  return 0;
}
