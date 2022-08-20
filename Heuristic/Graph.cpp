#include "Heuristic/Graph.h"
#include "DynamicGraph/Graph.h"
#include "Heuristic/DisjointSet.h"
#include "Random/Random.h"
#include <algorithm>
#include <bitset>
#include <cassert>
#include <cstdint>
#include <memory>
#include <sys/types.h>
#include <utility>

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
    IS >> U >> V;
    IS >> W;
    G.addBiEdge(U, V, W);
  }
  return IS;
}

const std::vector<Edge> &Graph::operator[](int Vertex) const {
  return getEdgesAt(Vertex);
}

const std::vector<Edge> &Graph::getEdgesAt(int Vertex) const {
  return EdgesOfNode[Vertex];
}

static constexpr bool inSet(int S, int Vertex) {
  return S & (1ll << (Vertex - 1));
}

static bool checkConnectivity(const Graph &G, const int S) {
  int BeginVertex = -1, VertexNum = G.getVertexNum();
  for (int Vertex = 1; Vertex <= VertexNum; Vertex++) {
    if (inSet(S, Vertex)) {
      BeginVertex = Vertex;
    }
  }
  if (BeginVertex == -1)
    return false; // 舍去空集的情况
  auto Visited = std::make_unique<bool[]>(VertexNum);
  for (int Vertex = 1; Vertex <= VertexNum; Vertex++) {
    Visited[Vertex] = false;
  }
  std::function<void(int)> DFS;
  DFS = [&](int U) {
    Visited[U] = true;
    for (const auto &E : G.getEdgesAt(U)) {
      auto &[V, _] = E;
      if (!Visited[V] && inSet(S, V)) {
        DFS(V);
      }
    }
  };

  DFS(BeginVertex);

  for (int Vertex = 1; Vertex <= VertexNum; Vertex++) {
    if (inSet(S, Vertex) && !Visited[Vertex])
      return false;
  }
  return true;
}

static float getCut(const Graph &G, int S) {
  float Ans = 0;
  for (int Vertex = 1; Vertex <= G.getVertexNum(); Vertex++)
    if (inSet(S, Vertex))
      for (const auto &[V, W] : G.getEdgesAt(Vertex))
        if (!inSet(S, V))
          Ans += W;
  return Ans;
}

static std::unique_ptr<std::set<int>> makeSet(const int S,
                                              const int VertexNum) {
  auto Ans = std::make_unique<std::set<int>>();
  for (int Vertex = 1; Vertex <= VertexNum; Vertex++) {
    if (inSet(S, Vertex)) {
      Ans->insert(Vertex);
    }
  }
  return Ans;
}

std::pair<float, std::unique_ptr<std::set<int>>> Graph::bruteForce() const {
  using std::uint64_t;
  float Ans = 1e-9;
  std::unique_ptr<std::set<int>> AnsSet;
  for (uint64_t S = 1; S < (1ll << VertexNum) - 1; S++) {
    if (checkConnectivity(*this, S) && checkConnectivity(*this, ~S)) {
      auto CandidateAns = getCut(*this, S);
      if (Ans < CandidateAns) {
        Ans = CandidateAns;
        AnsSet = makeSet(S, VertexNum);
      }
    }
  }
  return std::make_pair(Ans, std::move(AnsSet));
}

void MSTGraph::addBiEdge(int U, int V, float W) {
  Graph::addBiEdge(U, V, W);
  Edges->push_back(MSTEdge{U, V, W});
}

std::pair<std::unique_ptr<std::vector<MSTEdge>>,
          std::unique_ptr<std::vector<MSTEdge>>>
MSTGraph::calcMST() {
  std::sort(Edges->begin(), Edges->end(),
            [](const MSTEdge &E1, const MSTEdge &E2) { return E1.W < E2.W; });
  auto DS = std::make_unique<DisjointSet>(VertexNum);
  auto UnusedEdges = std::make_unique<std::vector<MSTEdge>>();
  auto UsedEdges = std::make_unique<std::vector<MSTEdge>>();
  for (const auto &E : *Edges) {
    auto &[U, V, W] = E;
    if (DS->isConnected(U, V))
      UnusedEdges->push_back(E);
    else {
      UsedEdges->push_back(E);
      DS->link(U, V);
    }
  }
  return std::make_pair(std::move(UsedEdges), std::move(UnusedEdges));
}

std::pair<float, std::unique_ptr<std::set<int>>> MSTGraph::solve() {
  float Ans = -1e8;
  auto AnsSet = std::make_unique<std::set<int>>();
  auto WorkSet = std::make_unique<std::set<int>>();

  MSTEdge CutEdge;

  for (int Vertex = 2; Vertex <= VertexNum; Vertex++) {
    WorkSet->insert(Vertex);
  }
  std::unique_ptr<std::vector<MSTEdge>> UsedEdges, UnusedEdges;
  std::tie(UsedEdges, UnusedEdges) = calcMST();

  auto DG = std::make_unique<DynamicGraph::Graph>(VertexNum);
  for (const auto &[U, V, _] : *UsedEdges) {
    DG->link(U, V);
  }

  for (const auto &E : *UsedEdges) {
    auto &[U, V, W] = E;
    DG->cut(U, V);
    float CurrentAns = W;
    for (const auto &[U_, V_, W_] : *UnusedEdges) {
      if (!DG->isConnected(U_, V_)) {
        CurrentAns += W_;
      }
    }
    if (CurrentAns > Ans) {
      Ans = CurrentAns;
      CutEdge = E;
    }
    DG->link(U, V);
  }

  DG->cut(CutEdge.U, CutEdge.V);

  // construct answer set

  for (int Vertex = 1; Vertex <= VertexNum; Vertex++) {
    if (DG->isConnected(Vertex, 1)) {
      AnsSet->insert(Vertex);
    }
  }

  return std::make_pair(Ans, std::move(AnsSet));
}
} // namespace Heuristic
