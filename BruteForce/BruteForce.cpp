#include <cstdint>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <string>
#define InMask(Mask, V) ((1ll << ((V)-1)) & Mask)
const int MaxVertexNum = 200;

struct Edge {
  int From, To, NextEdge;
  float Weight;
} Edges[MaxVertexNum * 2];

float Ans = 0;
int EdgeNum = 0;
int VertexNum;

int Head[MaxVertexNum];
int WhiteVertexes[MaxVertexNum], WhiteVertexNum;
int Color[MaxVertexNum];
std::uint64_t Mask, AnsMask;

void addEdge(int From, int To, float Weight) {
  Edges[EdgeNum].From = From;
  Edges[EdgeNum].To = To;
  Edges[EdgeNum].NextEdge = Head[From];
  Edges[EdgeNum].Weight = Weight;
  Head[From] = EdgeNum++;
}

void dfsColoring(int U, int Marker) {
  if (!InMask(Mask, U) && Marker == 1)
    return; // 如果在染A集合，不能染到外面去
  Color[U] = Marker;
  for (int I = Head[U]; ~I; I = Edges[I].NextEdge) {
    int V = Edges[I].To;
    if (Color[V])
      continue;
    dfsColoring(V, Marker);
  }
}

int checkDirection() {
  // 检查方向是否一致
  int DirectionFlag = -1; // -1: 未决定 0：朝外 1： 朝内。
  for (int VertexIdx = 0; VertexIdx < WhiteVertexNum; VertexIdx++) {
    int U = WhiteVertexes[VertexIdx];
    for (int EdgeIdx = Head[U]; ~EdgeIdx; EdgeIdx = Edges[EdgeIdx].NextEdge) {
      int V = Edges[EdgeIdx].To;
      if (InMask(Mask, V)) {
        continue; // 是集合A的内部边
      }
      if (~DirectionFlag) {
        if (DirectionFlag ^
            (Edges[EdgeIdx].Weight <
             0)) {  // flag == 0 (朝外) xor e[j].w < 0 (边朝内)  == 1
          return 0; // False
        }
      } else {
        // undefined
        DirectionFlag = Edges[EdgeIdx].Weight < 0;
      }
    }
  }
  return 1; // True
}

void calcCut() {
  int CutEdgeNum = 0;
  double WeightSum = 0;
  for (int K = 0; K < WhiteVertexNum; K++) {
    int Vertex = WhiteVertexes[K];
    for (int I = Head[Vertex]; ~I; I = Edges[I].NextEdge) {
      int V = Edges[I].To;
      if (!InMask(Mask, V)) {
        CutEdgeNum++;
        WeightSum += Edges[I].Weight;
      }
    }
  }
  double Weight = WeightSum;
  if (Weight > Ans) {
    Ans = Weight;
    AnsMask = Mask;
  }
}

void getAns() {
  std::uint64_t Max = (1ll << VertexNum) - 1;
  for (Mask = 1; Mask < Max; ++Mask) {
    if (Mask % 100000 == 0) {
      printf("\r%.2f%%", (double)Mask / Max * 100);
    }
    WhiteVertexNum = 0;
    for (int I = 1; I <= VertexNum; I++) {
      Color[I] = 0;
    }
    for (int I = 0; I < VertexNum; I++) {
      if (Mask & (1ll << I)) {
        WhiteVertexes[WhiteVertexNum++] = I + 1;
      }
    }
    dfsColoring(WhiteVertexes[0], 1);
    int OK = 1;
    for (int VertexIdx = 0; VertexIdx < WhiteVertexNum; VertexIdx++) {
      if (!Color[WhiteVertexes[VertexIdx]]) {
        OK = 0;
        break; // 不连通
      }
    }
    if (!OK)
      continue; // A不连通
    OK = 1;
    for (int I = 1; I <= VertexNum; I++) {
      if (!Color[I]) {
        // 应该属于 B 集合
        dfsColoring(I, 2);
        break; // 对一个节点进行dfs染色，之后就直接看是不是全部染成了1或者2
      }
    }
    for (int I = 1; I <= VertexNum; I++) {
      if (!Color[I]) {
        // 有被遗漏的节点，丢弃!
        OK = 0;
        break;
      }
    }
    if (!OK)
      continue; // B不连通
    if (!checkDirection())
      continue; // 方向不一致
    // OK, correct
    calcCut();
  }
}

void clearGraph() {
  memset(Head, 0xff, sizeof(Head));
  EdgeNum = 0;
}

void prettyPrintAnsSet(std::uint64_t Mask) {
  for (int Vertex = 1; Vertex <= VertexNum; Vertex++) {
    if (InMask(Mask, Vertex)) {
      printf("%d ", Vertex);
    }
  }
  puts("");
}

int main() {
  clearGraph();
  int EdgeInputNum;
  scanf("%d%d\n", &VertexNum, &EdgeInputNum);
  for (int I = 0; I < EdgeInputNum; I++) {
    int U, V;
    float W;
    scanf("%d%d%f", &U, &V, &W);
    addEdge(U, V, W);
    addEdge(V, U, W);
  }
  getAns();
  printf("%.3f\n", Ans);
  prettyPrintAnsSet(AnsMask);
  // print_union_result();
}
