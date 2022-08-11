#include "Heuristic/Graph.h"
#include "Random/Random.h"
#include <cassert>
#include <cstdlib>
#include <memory>
namespace Heuristic {
void Graph::addEdge(int u, int v, float w) {
  edgeNum++;
  edges[u].push_back(Edge{.v = v, .w = w});
}

std::unique_ptr<Graph> random_bigraph(int n_vertex, int n_edge) {
  auto ret = std::make_unique<Graph>(n_vertex);

  assert(n_edge + 1 >= n_vertex);

  // generate a tree at first
  // for edges: [1, n_vertex)
  for (int i = 2; i <= n_vertex; i++) {
    // u belongs to [1, i)
    // loop constraints: nodes between [1, i) connected before
    // so we add edge between i and randomly chosen node x
    // ensures [1, i] are connected then, and constraints
    // keeps to next iteration
    ret->addBiEdge(rand_range(1, i), i, rand_float());
  }

  // now all vertexes are connected (as a tree)
  // generates [n_vertex, n_edge) edges here
  for (int i = n_vertex; i < n_edge; i++) {
    // u, v belongs to [1, n_vertex + 1)
    int u = rand_range(1, n_vertex + 1);
    int v = rand_range(1, n_vertex + 1);
    ret->addBiEdge(u, v, rand_float());
  }

  return ret;
}

} // namespace Heuristic
