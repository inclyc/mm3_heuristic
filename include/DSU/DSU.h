#include <memory>
namespace DSU {
class DSU {
private:
  std::unique_ptr<int[]> fa;
  int findFa(int);

public:
  explicit DSU(int n);
  void link(int u, int v) { fa[static_cast<size_t>(findFa(u))] = findFa(v); }
  bool isConnected(int u, int v) { return findFa(u) == findFa(v); }
};
} // namespace DSU
