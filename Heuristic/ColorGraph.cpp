#include "Heuristic/ColorGraph.h"
#include <queue>
namespace Heuristic {
void ColorGraph::bfs() {
  std::queue<int> q;
  for (const auto u : *A) {
    q.push(u);
  }

  for (const auto u : *B) {
    q.push(u);
  }

  while (!q.empty()) {
    int u = q.front();
    bool inA = A->contains(u);
    q.pop();
    for (int i = head[static_cast<size_t>(u)]; i;
         i = edges[static_cast<size_t>(i)].next) {
      int v = edges[static_cast<size_t>(i)].v;
      if (A->contains(v) || B->contains(v)) {
        continue;
      }
      if (inA) {
        A->insert(v);
        q.push(v);
      } else {
        B->insert(v);
        q.push(v);
      }
    }
  }
}

bool ColorGraph::checkSetConn(const std::set<int> &s) const {
  std::set<int> visited;
  std::queue<int> q;
  for (const auto u : s) {
    visited.insert(u);
    q.push(u);
  }
  while (!q.empty()) {
    int u = q.front();
    q.pop();
    for (int i = head[static_cast<size_t>(u)]; i;
         i = edges[static_cast<size_t>(i)].next) {
      int v = edges[static_cast<size_t>(i)].v;
      if (s.contains(v) && !visited.contains(v)) {
        visited.insert(v);
        q.push(v);
      }
    }
  }
  return visited == s;
}

} // namespace Heuristic
