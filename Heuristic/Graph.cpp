#include "Heuristic/Graph.h"
#include "Random/Random.h"
#include <cassert>
#include <cstdlib>
#include <memory>
namespace Heuristic {
void Graph::addEdge(int U, int V, float W) {
  EdgeNum++;
  EdgesOfNode[U].push_back(Edge{.V = V, .W = W});
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

void Graph::setVertexNum(int VertexNum) {
  this->VertexNum = VertexNum;
  EdgesOfNode = std::make_unique<std::vector<Edge>[]>(VertexNum + 1);
}

std::istream &operator>>(std::istream &IS, Graph &G) {
  int VertexNum, EdgeNum;
  IS >> VertexNum >> EdgeNum;
  G.setVertexNum(VertexNum);
  for (int I = 0; I < EdgeNum; I++) {
    int U, V;
    float W;
    IS >> U >> V >> W;
    G.addBiEdge(U, V, W);
  }
  return IS;
}

const std::vector<Edge> &Graph::operator[](int Vertex) const {
  return EdgesOfNode[Vertex];
}

} // namespace Heuristic
