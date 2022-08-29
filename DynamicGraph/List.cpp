#include "DynamicGraph/List.h"
#include <cstddef>
#include <memory>
namespace DynamicGraph {
void List::init(std::size_t VertexNum) {
  Head = std::make_unique<int[]>(VertexNum + 1);
  Precessor = std::make_unique<int[]>(VertexNum + 1);
  Successor = std::make_unique<int[]>(VertexNum + 1);
  for (unsigned int I = 0; I <= VertexNum; ++I) {
    Head[I] = Precessor[I] = Successor[I] = 0;
  }
}

void List::cut(int U, int V) {
  if (Successor[V] == V) {
    Head[U] = 0;
  } else {
    if (Head[U] == V) {
      Head[U] = Successor[V];
    }
    Precessor[Successor[V]] = Precessor[V];
    Successor[Precessor[V]] = Successor[V];
  }
}

void List::link(int U, int V) {
  if (!Head[U]) {
    Head[U] = Precessor[V] = Successor[V] = V;
  } else {
    int X = Head[U];
    int Y = Precessor[X];
    Precessor[X] = Successor[Y] = V;
    Precessor[V] = Y;
    Successor[V] = X;
  }
}

bool List::empty(int U) { return Head[U] == 0; }

int List::first(int U) { return Head[U]; }
} // namespace DynamicGraph
