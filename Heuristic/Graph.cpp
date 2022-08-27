#include "Heuristic/Graph.h"
#include "DynamicGraph/Graph.h"
#include "Heuristic/DisjointSet.h"
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <fmt/core.h>
#include <functional>
#include <memory>
#include <sys/types.h>
#include <utility>

namespace Heuristic {
void Graph::addEdge(int U, int V, float W) {
  EdgesOfNode[U].push_back(Edge{.V = V, .W = W});
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

std::ostream &operator<<(std::ostream &OS, const Graph &G) {
  OS << G.VertexNum << " " << G.EdgeNum << std::endl;
  int EdgeCounter = 0;
  for (int Vertex = 1; Vertex <= G.VertexNum; Vertex++) {
    for (const auto &E : G.EdgesOfNode[Vertex]) {
      assert(Vertex != E.V);
      if (Vertex < E.V) {
        OS << Vertex << " " << E.V << " " << E.W << std::endl;
        EdgeCounter++;
      }
    }
  }
  assert(EdgeCounter == G.EdgeNum);
  return OS;
}

const std::vector<Edge> &Graph::operator[](int Vertex) const {
  return getEdgesAt(Vertex);
}

const std::vector<Edge> &Graph::getEdgesAt(int Vertex) const {
  return EdgesOfNode[Vertex];
}

void Graph::printSetByEdge(const std::set<int> &S) const {
  for (int U = 1; U <= VertexNum; U++) {
    for (const auto &[V, W] : EdgesOfNode[U]) {
      if (S.contains(U) && !S.contains(V))
        fmt::print("{} {} {}\n", U, V, W);
    }
  }
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
    return false; // Ignore empty set
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
  for (uint64_t S = 1; S < (1ull << VertexNum) - 1; S++) {
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
  EdgeNum += 1;
}

std::pair<std::unique_ptr<std::vector<MSTEdge>>,
          std::unique_ptr<std::vector<MSTEdge>>>
MSTGraph::spanningTree() {
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

std::pair<float, std::unique_ptr<std::set<int>>> MSTGraph::solve(int TestNum) {
  float Ans = -1e8;
  auto AnsSet = std::make_unique<std::set<int>>();
  auto WorkSet = std::make_unique<std::set<int>>();

  MSTEdge CutEdge;

  for (int Vertex = 2; Vertex <= VertexNum; Vertex++) {
    WorkSet->insert(Vertex);
  }
  std::unique_ptr<std::vector<MSTEdge>> UsedEdges, UnusedEdges;
  std::shared_ptr<DynamicGraph::Graph> DG, AnsDG;
  std::sort(Edges->begin(), Edges->end(),
            [](const MSTEdge &E1, const MSTEdge &E2) { return E1.W < E2.W; });
  for (int K = 0; K < TestNum; K++) {
    int RandNum = Edges->size() * K / TestNum;
    std::random_shuffle(Edges->begin(), Edges->begin() + RandNum);
    DG = std::make_shared<DynamicGraph::Graph>(VertexNum);
    std::tie(UsedEdges, UnusedEdges) = spanningTree();
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
        AnsDG = DG;
      }
      DG->link(U, V);
    }
  }
  // construct answer set
  AnsDG->cut(CutEdge.U, CutEdge.V);
  for (int Vertex = 1; Vertex <= VertexNum; Vertex++) {
    if (AnsDG->isConnected(Vertex, 1)) {
      AnsSet->insert(Vertex);
    }
  }

  return std::make_pair(Ans, std::move(AnsSet));
}
} // namespace Heuristic
