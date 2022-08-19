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

  // Visit & mark each nodes could be reached from U
  std::function<void(int)> DFS;
  DFS = [&](int U) {
    IsVisited[U] = true;
    for (const auto &[V, _] : Edges[U]) {
      if (!IsVisited[V] && Color[V] == WorkColor && !CurrentSet->contains(V)) {
        DFS(V);
      }
    }
  };

  /// @param U check if all nodes colored as `WorkColor' are connected
  auto CheckConnectivity = [&](int U) {
    // reset visited array for DFS
    // in DFS search, we mark nodes we can reach a label "visited"
    for (int I = 0; I <= VertexNum; I++) {
      IsVisited[I] = false;
    }
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
      if (!CurrentSet->contains(V) && Color[V] == WorkColor) {
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
    if (Ans < CurrentAns && CheckConnectivity(U)) {
      Ans = CurrentAns;
      // Copy the set because it may have some changes later
      // std::make_shared uses copy constructor
      AnsSet = std::make_shared<std::set<int>>(*CurrentSet);
    }

    // Not inner edges, this edge points to a new vertex U
    // insert U to current set and update current answer
    CurrentSet->insert(U);
    for (const auto &E : Edges[U]) {
      const auto &[V, W] = E;
      if (CurrentSet->contains(V)) {
        CurrentAns -= W;
      } else if (Color[V] != WorkColor) {
        continue;
      } else {
        CurrentAns += W;
        PQ.push(E);
      }
    }
  }
  return std::make_pair(Ans, AnsSet);
}

std::tuple<float, int, std::shared_ptr<std::set<int>>>
ColorGraph::solveArticulation(int A) {
  /*     +-----+
         | BCC |  <-- (color 2) <-- must be in the same set as U
         *-----*
          \   /
           \ /
            *   <------   U (Articulation Point)
           / \
          /   \
         *-----* (color 1)
         | BCC |   <-- consider this BCC contains different color,
         +-----+
  */
  float Ans = -1e5f;
  assert(ArticulationPoints.contains(A));
  colorArticulationPoint(A);
  std::shared_ptr<std::set<int>> AnsSet;
  auto ColorSet = std::make_shared<std::set<int>>();
  int AnsWorkColor = 0;
  // At vertex u, map color -> max cut
  std::map<int, float> ArticulationCut;
  for (const auto &E : Edges[A]) {
    if (E.V == A)
      continue; // self loops
    ColorSet->insert(Color[E.V]);
  }
  for (const auto &WorkColor : *ColorSet) {
    auto [CandidateAns, CandidateAnsSet] = solveArticulation(A, WorkColor);
    if (Ans < CandidateAns) {
      Ans = CandidateAns;
      AnsSet = CandidateAnsSet;
      AnsWorkColor = WorkColor;
    }
  }
  return std::make_tuple(Ans, AnsWorkColor, AnsSet);
}

std::pair<float, std::shared_ptr<std::set<int>>> ColorGraph::solve() {
  float Ans = -1e5f;
  std::shared_ptr<std::set<int>> AnsSet;
  int AnsWorkColor = 0;
  int AnsArticulationPoint = 0;
  getArticulationPoints();
  for (const auto &U : ArticulationPoints) {
    auto [CandidateAns, CandidateWorkColor, CandidateSet] =
        solveArticulation(U);
    if (Ans < CandidateAns) {
      Ans = CandidateAns;
      AnsSet = CandidateSet;
      AnsArticulationPoint = U;
      AnsWorkColor = CandidateWorkColor;
    }
  }
  return std::make_pair(
      Ans, constructFinalAnswer(AnsArticulationPoint, AnsWorkColor, AnsSet));
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

std::shared_ptr<std::set<int>>
ColorGraph::constructFinalAnswer(int A, int WorkColor,
                                 std::shared_ptr<std::set<int>> Chosen) {
  colorArticulationPoint(A);
  dumpColor();
  auto AnsSet = std::make_shared<std::set<int>>();
  auto IsVisited = std::make_unique<bool[]>(VertexNum + 1); // For DFS

  // prepare visited array for DFS
  for (int Vertex = 1; Vertex <= VertexNum; Vertex++) {
    IsVisited[Vertex] = false;
  }

  // Visit & mark each nodes could be reached from U (only work color)
  // Insert vertex v if it is colored as work color, and not be chosen
  std::function<void(int)> DFS;
  DFS = [&](int U) {
    IsVisited[U] = true;
    if (Color[U] == WorkColor && !Chosen->contains(U)) {
      AnsSet->insert(U);
    }
    for (const auto &[V, _] : Edges[U]) {
      if (!IsVisited[V] && Color[V] == WorkColor) {
        DFS(V);
      }
    }
  };

  DFS(A);

  return AnsSet;
}

} // namespace Heuristic
