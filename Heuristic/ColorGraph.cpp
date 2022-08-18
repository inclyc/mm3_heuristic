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
int ColorGraph::getColor() {
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
          Color[U] = -1;
        }
      } else if (V != Fa) {
        LowestReach[U] = std::min(LowestReach[U], DfsNumber[V]);
      }
    }
    if (U == Fa && Child > 1) {
      ArticulationPoints.insert(U);
      Color[U] = -1;
    }
  };
  // by Robert E. Tarjan
  // O(n)
  for (int I = 1; I <= VertexNum; I++) {
    if (!IsVisited[I]) {
      Tarjan(I, I);
    }
  }
  // Assign each node in BCC an ID and stores in 'color'
  // the color of some vertexes u is '-1', indicates that
  // these vertexes are articulation points
  Count = 0; // for allocating ID
  std::function<void(int)> Dfs;

  // give node u as color `cnt`, and walk through dfs tree recursively
  // don't step in articulation or colored points.
  Dfs = [&](int U) {
    Color[U] = Count;
    for (const auto &E : Edges[U]) {
      int V = E.V;
      if (!Color[V]) {
        // i.e. we haven't assigned a color to v
        Dfs(V);
      }
    }
  };

  for (int I = 1; I <= VertexNum; I++) {
    if (!Color[I]) {
      ++Count;
      Dfs(I);
    }
  }
  return ArticulationPoints.size();
}

std::pair<float, std::shared_ptr<std::set<int>>>
ColorGraph::solveSet(std::shared_ptr<std::set<int>> S) {
  float CurrentAnswer = 0.0f;
  float Answer = -1e5f;

  // Each time we get a minimum edge from all available edge in this queue
  std::priority_queue<Edge> PriorityQueue;

  // Should return this
  std::shared_ptr<std::set<int>> AnsSet;

  // A set of vertexes we considering now
  auto CurrentSet = std::make_shared<std::set<int>>(*S);
  for (const auto &U : *S) {
    for (const auto &E : Edges[U]) {
      auto [V, W] = E;
      if (!CurrentSet->contains(V) && Color[V] == Color[U]) {
        PriorityQueue.push(E);
        CurrentAnswer += W;
      }
    }
  }
  while (!PriorityQueue.empty()) {
    if (Answer < CurrentAnswer) {
      Answer = CurrentAnswer;
      // Copy TempSet because it may have some changes later
      // std::make_shared uses copy constructor
      AnsSet = std::make_shared<std::set<int>>(*CurrentSet);
    }
    // Minimum weight edge we could use
    const auto &CurrentMinEdge = PriorityQueue.top();
    PriorityQueue.pop();
    int U = CurrentMinEdge.V;
    if (CurrentSet->contains(U))
      continue;

    // Not inner edges, this edge points to a new vertex U
    // insert U to current set and update current answer
    CurrentSet->insert(U);
    for (const auto &E : Edges[U]) {
      if (Color[E.V] != Color[U] || E.V == U)
        continue;
      if (CurrentSet->contains(E.V)) {
        CurrentAnswer -= E.W;
      } else {
        CurrentAnswer += E.W;
        PriorityQueue.push(E);
      }
    }
  }
  return std::make_pair(Answer, AnsSet);
}

std::pair<float, std::shared_ptr<std::set<int>>>
ColorGraph::solveArticulation(int U) {
  /*
         * ----*  (color 2)
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
  std::shared_ptr<std::set<int>> AnsSet;
  // At vertex u, map color -> adjacent vertexes
  std::map<int, std::shared_ptr<std::set<int>>> AdjacentVertexMap;

  // At vertex u, map color -> max cut
  std::map<int, float> ArticulationCut;
  for (const auto &E : Edges[U]) {
    if (E.V == U)
      continue; // self loops
    if (!AdjacentVertexMap.contains(Color[E.V])) {
      AdjacentVertexMap.insert({Color[E.V], std::make_shared<std::set<int>>()});
      ArticulationCut.insert({Color[E.V], 0});
    }
    AdjacentVertexMap[Color[E.V]]->insert(E.V);
    ArticulationCut[Color[E.V]] += E.W;
  }
  for (const auto &[U, MaxCutOfU] : ArticulationCut) {
    if (Ans < MaxCutOfU) {
      Ans = MaxCutOfU;
      AnsSet = AdjacentVertexMap[U];
    }
  }
  for (const auto &[U, AdjacentSet] : AdjacentVertexMap) {
    auto [CandidateAns, CandidateAnsSet] = solveSet(AdjacentSet);
    if (Ans < CandidateAns) {
      Ans = CandidateAns;
      AnsSet = AdjacentSet;
    }
  }
  return std::make_pair(Ans, AnsSet);
}

std::pair<float, std::shared_ptr<std::set<int>>> ColorGraph::solve() {
  float Ans = -1e5f;
  std::shared_ptr<std::set<int>> AnsSet;
  getColor();
  for (const auto &U : ArticulationPoints) {
    auto [CandidateAns, CandidateSet] = solveArticulation(U);
    if (Ans < CandidateAns) {
      Ans = CandidateAns;
      AnsSet = CandidateSet;
    }
  }
  return std::make_pair(Ans, AnsSet);
}

} // namespace Heuristic
