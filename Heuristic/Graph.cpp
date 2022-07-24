#include "Heuristic/Graph.h"
#include <cassert>
#include <cstdlib>
#include <memory>
namespace Heuristic {
void Graph::addEdge(int u, int v, float w) {
  edgeNum++;
  auto &hdu = head[static_cast<size_t>(u)];
  edges[static_cast<size_t>(edgeNum)] =
      Edge{.u = u, .v = v, .next = hdu, .w = w};
  hdu = edgeNum;
}

Graph::Graph(int v, int e) {
  edges = std::make_unique<Edge[]>(static_cast<size_t>(e + 1));
  head = std::make_unique<int[]>(static_cast<size_t>(v + 1));
  edgeNum = 0;
  vertexNum = v;
}

/**
 * @brief helper function for generates integer in [l, r)
 *
 * @param l lower bound (inclusive)
 * @param r upper bound (exclusive)
 * @return the result
 */
static int rand_range(int l, int r) { return rand() % (r - l) + l; }

/**
 * @brief random number between [0, 1)
 *
 * @return float
 */
static float rand_float() {
  return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

std::unique_ptr<Graph> random_bigraph(int n_vertex, int n_edge) {
  auto ret = std::make_unique<Graph>(n_vertex, n_edge * 2);

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
