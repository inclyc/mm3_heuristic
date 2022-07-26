#pragma once
#include "Random/Random.h"
#include <cassert>
#include <istream>
#include <memory>
#include <ostream>
#include <set>
#include <vector>
namespace Heuristic {

struct Edge {
  int V;
  float W;

  bool operator<(const Edge &E) const { return W > E.W; }
};

class Graph {
protected:
  std::unique_ptr<std::vector<Edge>[]> EdgesOfNode;
  int VertexNum, EdgeNum;

public:
  Graph(int V) {
    EdgesOfNode = std::make_unique<std::vector<Edge>[]>(V + 1);
    VertexNum = V;
    EdgeNum = 0;
  }

  Graph() {
    EdgeNum = 0;
    VertexNum = 0;
  }
  virtual void setVertexNum(int VertexNum);
  int getVertexNum() const { return VertexNum; }
  void addEdge(int U, int V, float W);
  virtual void addBiEdge(int U, int V, float W) {
    EdgeNum++;
    addEdge(U, V, W);
    addEdge(V, U, W);
  }

  friend std::istream &operator>>(std::istream &IS, Graph &G);

  const std::vector<Edge> &operator[](int Vertex) const;

  const std::vector<Edge> &getEdgesAt(int Vertex) const;

  void printSetByEdge(const std::set<int> &S) const;

  std::pair<float, std::unique_ptr<std::set<int>>> bruteForce() const;

  friend std::ostream &operator<<(std::ostream &OS, const Graph &G);
};

template <class T>
std::unique_ptr<T> randomBiGraph(int VertexNum, int EdgeNum) {
  auto Ans = std::make_unique<T>(VertexNum);

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
  std::set<std::pair<int, int>> EdgeCovered;

  // now all vertexes are connected (as a tree)
  // generates [VertexNum, EdgeNum) edges here
  for (int I = VertexNum; I <= EdgeNum; I++) {
    // U, V belongs to [1, VertexNum + 1)
    int U = 0, V = 0;
    for (;;) {
      U = randRange(1, VertexNum + 1);
      V = randRange(1, VertexNum + 1);
      if (U == V)
        continue; // no self loops
      auto E = std::make_pair(U, V);
      if (EdgeCovered.contains(E))
        continue; // no parallel edges

      EdgeCovered.insert(E); // Add this edge to covered set
      Ans->addBiEdge(U, V, randFloat());
      break; // OK, fine
    }
  }

  return Ans;
}

struct MSTEdge {
  int U, V;
  float W;
};

class MSTGraph : public Graph {
private:
  std::unique_ptr<std::vector<MSTEdge>> Edges;

public:
  void addBiEdge(int U, int V, float W) override;
  MSTGraph() : Graph() { Edges = std::make_unique<std::vector<MSTEdge>>(); }
  MSTGraph(int VertexNum) : Graph(VertexNum) {
    Edges = std::make_unique<std::vector<MSTEdge>>();
  }

  /// @return (UsedEdges, UnusedEdges)
  std::pair<std::unique_ptr<std::vector<MSTEdge>>,
            std::unique_ptr<std::vector<MSTEdge>>>
  spanningTree(const std::vector<MSTEdge> &LocalEdges);

  std::pair<float, std::unique_ptr<std::set<int>>> solve(int TestNum);
};

} // namespace Heuristic
