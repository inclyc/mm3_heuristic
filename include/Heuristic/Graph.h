#pragma once
#include "Random/Random.h"
#include <cassert>
#include <istream>
#include <memory>
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
  }

  Graph() {}
  virtual void setVertexNum(int VertexNum);
  int getVertexNum() const { return VertexNum; }
  void addEdge(int U, int V, float W);
  virtual void addBiEdge(int U, int V, float W) {
    addEdge(U, V, W);
    addEdge(V, U, W);
  }

  friend std::istream &operator>>(std::istream &IS, Graph &G);

  const std::vector<Edge> &operator[](int Vertex) const;

  const std::vector<Edge> &getEdgesAt(int Vertex) const;

  std::pair<float, std::unique_ptr<std::set<int>>> bruteForce() const;
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
  calcMST();

  std::pair<float, std::unique_ptr<std::set<int>>> solve();
};

} // namespace Heuristic
