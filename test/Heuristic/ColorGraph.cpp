#include "Heuristic/ColorGraph.h"
#include "Heuristic/Graph.h"
#include <memory>

int main() {
  auto p = std::make_unique<Heuristic::ColorGraph>(5);
  p->addBiEdge(1, 2, 1);
  p->addBiEdge(1, 3, 1);
  p->addBiEdge(2, 3, 1);
  p->addBiEdge(3, 4, 1);
  p->addBiEdge(3, 5, 1);
  p->addBiEdge(4, 5, 1);
  p->getColor();
}