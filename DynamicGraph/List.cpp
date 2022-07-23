#include "DynamicGraph/List.h"
#include <cstddef>
namespace DynamicGraph {
void List::init(std::size_t n) {
  head = new int[n + 1]();
  pre = new int[n + 1]();
  next = new int[n + 1]();
  for (unsigned int i = 0; i <= n; ++i) {
    head[i] = pre[i] = next[i] = 0;
  }
}

void List::free() {
  delete[] head;
  delete[] pre;
  delete[] next;
}

void List::cut(int u, int v) {
  if (next[v] == v) {
    head[u] = 0;
  } else {
    if (head[u] == v) {
      head[u] = next[v];
    }
    v[next][pre] = v[pre];
    v[pre][next] = v[next];
  }
}

void List::link(int u, int v) {
  if (!head[u]) {
    head[u] = pre[v] = next[v] = v;
  } else {
    int x = head[u];
    int y = pre[x];
    pre[x] = next[y] = v;
    pre[v] = y;
    next[v] = x;
  }
}

bool List::empty(int u) { return head[u] == 0; }

int List::first(int u) { return head[u]; }
} // namespace DynamicGraph
