#pragma once
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
};

/**
 * @brief generates a random bi-connected graph
 *
 * @param n_vertex number of vertex
 * @param n_edge number of edge
 * @note ensure e >= v - 1
 * @return std::unique_ptr<Graph>
 */
std::unique_ptr<Graph> randomBiGraph(int VertexNum, int EdgeNum);

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

  /// @return (UsedEdges, UnusedEdges)
  std::pair<std::unique_ptr<std::vector<MSTEdge>>,
            std::unique_ptr<std::vector<MSTEdge>>>
  calcMST();

  std::pair<float, std::unique_ptr<std::set<int>>> solve();
};

} // namespace Heuristic
