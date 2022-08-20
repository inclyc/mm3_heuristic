#include "DynamicGraph/List.h"
#include <cstddef>
namespace DynamicGraph {
void List::init(std::size_t VertexNum) {
  head = new int[VertexNum + 1]();
  pre = new int[VertexNum + 1]();
  next = new int[VertexNum + 1]();
  for (unsigned int I = 0; I <= VertexNum; ++I) {
    head[I] = pre[I] = next[I] = 0;
  }
}

void List::free() {
  delete[] head;
  delete[] pre;
  delete[] next;
}

void List::cut(int U, int V) {
  if (next[V] == V) {
    head[U] = 0;
  } else {
    if (head[U] == V) {
      head[U] = next[V];
    }
    V[next][pre] = V[pre];
    V[pre][next] = V[next];
  }
}

void List::link(int U, int V) {
  if (!head[U]) {
    head[U] = pre[V] = next[V] = V;
  } else {
    int X = head[U];
    int Y = pre[X];
    pre[X] = next[Y] = V;
    pre[V] = Y;
    next[V] = X;
  }
}

bool List::empty(int U) { return head[U] == 0; }

int List::first(int U) { return head[U]; }
} // namespace DynamicGraph
