#include "Heuristic/Graph.h"
#include "Random/Random.h"
#include <cassert>
#include <cstdlib>
#include <memory>
namespace Heuristic {
void Graph::addEdge(int U, int V, float W) {
  EdgeNum++;
  Edges[U].push_back(Edge{.V = V, .W = W});
}

std::unique_ptr<Graph> randomBiGraph(int VertexNum, int EdgeNum) {
  auto Ans = std::make_unique<Graph>(VertexNum);

  assert(EdgeNum + 1 >= VertexNum);

  // generate a tree at first
  // for edges: [1, VertexNum)
  for (int I = 2; I <= VertexNum; I++) {
    // u belongs to [1, I)
    // loop constraints: nodes between [1, I) connected before
    // so we add edge between I and randomly chosen node X
    // ensures [1, I] are connected then, and constraints
    // keeps to next iteration
    Ans->addBiEdge(randRange(1, I), I, randFloat());
  }

  // now all vertexes are connected (as a tree)
  // generates [VertexNum, EdgeNum) edges here
  for (int I = VertexNum; I < EdgeNum; I++) {
    // U, V belongs to [1, VertexNum + 1)
    int U = randRange(1, VertexNum + 1);
    int V = randRange(1, VertexNum + 1);
    Ans->addBiEdge(U, V, randFloat());
  }

  return Ans;
}

} // namespace Heuristic
