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
  // for edges: [1, n_vertex)
  for (int I = 2; I <= VertexNum; I++) {
    // u belongs to [1, i)
    // loop constraints: nodes between [1, i) connected before
    // so we add edge between i and randomly chosen node x
    // ensures [1, i] are connected then, and constraints
    // keeps to next iteration
    Ans->addBiEdge(rand_range(1, I), I, rand_float());
  }

  // now all vertexes are connected (as a tree)
  // generates [n_vertex, n_edge) edges here
  for (int I = VertexNum; I < EdgeNum; I++) {
    // u, v belongs to [1, n_vertex + 1)
    int U = rand_range(1, VertexNum + 1);
    int V = rand_range(1, VertexNum + 1);
    Ans->addBiEdge(U, V, rand_float());
  }

  return Ans;
}

} // namespace Heuristic
