#include "Heuristic/ColorGraph.h"
#include "Heuristic/Graph.h"
#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <queue>
#include <utility>
#include <vector>
namespace Heuristic {
int ColorGraph::getArticulationPoints() {
  auto LowestReach = std::make_unique<int[]>(VertexNum + 1);
  auto DfsNumber = std::make_unique<int[]>(VertexNum + 1);
  auto IsVisited = std::make_unique<bool[]>(VertexNum + 1);
  ArticulationPoints.clear();
  for (int I = 0; I < VertexNum + 1; I++) {
    LowestReach[I] = DfsNumber[I] = 0;
    IsVisited[I] = false;
  }
  int Count = 1;
  std::function<void(int, int)> Tarjan;
  Tarjan = [&](int U, int Fa) {
    IsVisited[U] = true;
    LowestReach[U] = DfsNumber[U] = Count++;
    int Child = 0;
    for (const auto &E : Edges[U]) {
      int V = E.V;
      if (!IsVisited[V]) {
        Child++;
        Tarjan(V, U);
        LowestReach[U] = std::min(LowestReach[U], LowestReach[V]);
        if (Fa != U && LowestReach[V] >= DfsNumber[U]) {
          ArticulationPoints.insert(U);
        }
      } else if (V != Fa) {
        LowestReach[U] = std::min(LowestReach[U], DfsNumber[V]);
      }
    }
    if (U == Fa && Child > 1) {
      ArticulationPoints.insert(U);
    }
  };
  // by Robert E. Tarjan
  // O(n)
  for (int I = 1; I <= VertexNum; I++) {
    if (!IsVisited[I]) {
      Tarjan(I, I);
    }
  }
  return ArticulationPoints.size();
}

void ColorGraph::colorArticulationPoint(int A) {
  // Assign each node in BCC an ID and stores in 'color'
  // the color of some vertexes u is '-1', indicates that
  // these vertexes are articulation points
  int N = 0; // for allocating ID
  auto IsVisited = std::make_unique<bool[]>(VertexNum + 1);
  for (int I = 0; I <= VertexNum; I++) {
    Color[I] = 0;
    IsVisited[I] = false;
  }
  Color[A] = -1;
  IsVisited[A] = true;
  // give node u as color `cnt`, and walk through dfs tree recursively
  // don't step in articulation or colored points.
  std::function<void(int)> DFS;
  DFS = [&](int U) {
    IsVisited[U] = true;
    Color[U] = N;
    for (const auto &E : Edges[U]) {
      int V = E.V;
      if (!IsVisited[V]) {
        // i.e. we haven't assigned a color to v
        DFS(V);
      }
    }
  };

  for (const auto &[U, _] : Edges[A]) {
    if (!IsVisited[U]) {
      N++;
      DFS(U);
    }
  }
}

std::pair<float, std::shared_ptr<std::set<int>>>
ColorGraph::solveArticulation(int A, int WorkColor) {
  float CurrentAns = 0.0f;
  float Ans = -1e5f;
  // A set of vertexes we considering now
  auto CurrentSet = std::make_shared<std::set<int>>();
  CurrentSet->insert(A);

  // Each time we get a minimum edge from all available edge in this queue
  std::priority_queue<Edge> PQ;
  // Should return this
  std::shared_ptr<std::set<int>> AnsSet;

  auto IsVisited = std::make_unique<int[]>(VertexNum + 1); // For DFS
  std::function<void(int)> DFS;
  DFS = [&](int U) {
    IsVisited[U] = true;
    for (const auto &[V, _] : Edges[U]) {
      if (!IsVisited[V] && Color[V] == WorkColor && !CurrentSet->contains(V)) {
        DFS(V);
      }
    }
  };

  auto ClearVisited = [&]() {
    for (int I = 0; I <= VertexNum; I++) {
      IsVisited[I] = false;
    }
  };

  auto CheckConnectivity = [&](int U) {
    ClearVisited();
    DFS(U);
    for (int I = 1; I <= VertexNum; I++) {
      if (!IsVisited[I] && Color[I] == WorkColor && !CurrentSet->contains(I)) {

        return false;
      }
    }
    return true;
  };

  for (const auto &U : *CurrentSet) {
    for (const auto &E : Edges[U]) {
      auto [V, W] = E;
      if (!CurrentSet->contains(V) && Color[V] == Color[U]) {
        PQ.push(E);
        CurrentAns += W;
      }
    }
  }
  while (!PQ.empty()) {

    // Minimum weight edge we could use
    const auto &CurrentMinEdge = PQ.top();
    PQ.pop();
    int U = CurrentMinEdge.V;
    if (CurrentSet->contains(U))
      continue;

    assert(Color[U] == WorkColor);
    if (CheckConnectivity(U) && Ans < CurrentAns) {
      Ans = CurrentAns;
      // Copy TempSet because it may have some changes later
      // std::make_shared uses copy constructor
      AnsSet = std::make_shared<std::set<int>>(*CurrentSet);
    }

    // Not inner edges, this edge points to a new vertex U
    // insert U to current set and update current answer
    CurrentSet->insert(U);
    for (const auto &E : Edges[U]) {
      const auto &[V, W] = E;
      if (Color[V] != Color[U] || V == U)
        continue;
      if (CurrentSet->contains(V)) {
        CurrentAns -= W;
      } else {
        CurrentAns += W;
        PQ.push(E);
      }
    }
  }
  return std::make_pair(Ans, AnsSet);
}

std::pair<float, std::shared_ptr<std::set<int>>>
ColorGraph::solveArticulation(int A) {
  /*
         *-----*  (color 2)
          \   /
           \ /
            *   <------   U (Articulation Point)
           / \      <--------   max cut here
          /   \
         *-----* (color 1)
         | BCC |
         +-----+
  */
  float Ans = -1e5f;
  assert(ArticulationPoints.contains(A));
  colorArticulationPoint(A);
  std::shared_ptr<std::set<int>> AnsSet;
  // At vertex u, map color -> adjacent vertexes
  std::map<int, std::shared_ptr<std::set<int>>> AdjacentVertexMap;

  // At vertex u, map color -> max cut
  std::map<int, float> ArticulationCut;
  for (const auto &E : Edges[A]) {
    if (E.V == A)
      continue; // self loops
    if (!AdjacentVertexMap.contains(Color[E.V])) {
      AdjacentVertexMap.insert({Color[E.V], std::make_shared<std::set<int>>()});
      ArticulationCut.insert({Color[E.V], 0});
    }
    AdjacentVertexMap[Color[E.V]]->insert(E.V);
    ArticulationCut[Color[E.V]] += E.W;
  }
  for (const auto &[AdjacentColor, MaxCutOfU] : ArticulationCut) {
    if (Ans < MaxCutOfU) {
      Ans = MaxCutOfU;
      AnsSet = AdjacentVertexMap[AdjacentColor];
    }
  }
  for (const auto &[AdjacentColor, AdjacentSet] : AdjacentVertexMap) {
    auto [CandidateAns, CandidateAnsSet] = solveArticulation(A, AdjacentColor);
    if (Ans < CandidateAns) {
      Ans = CandidateAns;
      AnsSet = CandidateAnsSet;
    }
  }
  return std::make_pair(Ans, AnsSet);
}

std::pair<float, std::shared_ptr<std::set<int>>> ColorGraph::solve() {
  float Ans = -1e5f;
  std::shared_ptr<std::set<int>> AnsSet;
  getArticulationPoints();
  for (const auto &U : ArticulationPoints) {
    auto [CandidateAns, CandidateSet] = solveArticulation(U);
    if (Ans < CandidateAns) {
      Ans = CandidateAns;
      AnsSet = CandidateSet;
    }
  }
  return std::make_pair(Ans, AnsSet);
}

void ColorGraph::dumpArticulationPoints() {
  std::cout << "Articulation Points:" << std::endl;
  for (const auto &U : ArticulationPoints) {
    std::cout << U << " ";
  }
  std::cout << std::endl;
}

void ColorGraph::dumpColor() {
  std::cout << "Color:" << std::endl;
  for (int I = 1; I <= VertexNum; I++) {
    std::cout << I << " " << Color[I] << std::endl;
  }
}

void ColorGraph::setVertexNum(int VertexNum) {
  Graph::setVertexNum(VertexNum);
  Color = std::make_unique<int[]>(VertexNum + 1);
}

} // namespace Heuristic
