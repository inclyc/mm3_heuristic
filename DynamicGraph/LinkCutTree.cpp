#include "DynamicGraph/LinkCutTree.h"
#include <memory>
#include <unordered_map>
#include <unordered_set>

namespace DynamicGraph {

#define l(u) ChildVertex[u][0]
#define r(u) ChildVertex[u][1]

void LinkCutTree::init(int VertexNum) {
  FatherVertex = std::make_unique<int[]>(VertexNum + 1);
  FlippedFlag = std::make_unique<int[]>(VertexNum + 1);
  Stack = std::make_unique<int[]>(VertexNum + 1);
  Size = std::make_unique<int[]>(VertexNum + 1);
  SizeValue = std::make_unique<int[]>(VertexNum + 1);

  for (std::size_t I = 0; I < 2; ++I) {
    ChildV[I].init(VertexNum);
    for (int U = 1; U <= VertexNum; ++U)
      G[U][I].clear();
  }
  for (int U = 0; U <= VertexNum; ++U) {
    SizeValue[U] = 0;
    Stack[U] = 0;
    FatherVertex[U] = l(U) = r(U) = Tag[U][0] = Tag[U][1] = FlippedFlag[U] =
        SizeValue[U] = 0;
    Size[U] = 1;
  }
  Size[0] = 0;
}

void LinkCutTree::linkv(int U, int V) {
  if (!U || !V)
    return;
  SizeValue[U] += Size[V];
  for (int I = 0; I < 2; ++I)
    if (Tag[V][I])
      ChildV[I].link(U, V);
}
void LinkCutTree::cutv(int U, int V) {
  if (!U || !V)
    return;
  SizeValue[U] -= Size[V];
  for (int I = 0; I < 2; ++I)
    if (Tag[V][I])
      ChildV[I].cut(U, V);
}
void LinkCutTree::rev(int U) {
  FlippedFlag[U] ^= 1;
  std::swap(l(U), r(U));
}

void LinkCutTree::pu(int U) {
  if (!U)
    return;
  for (int I = 0; I < 2; ++I)
    Tag[U][I] =
        Tag[l(U)][I] | Tag[r(U)][I] | !G[U][I].empty() | !ChildV[I].empty(U);
  Size[U] = Size[l(U)] + Size[r(U)] + 1 + SizeValue[U];
}

void LinkCutTree::pd(int U) {
  if (FlippedFlag[U]) {
    rev(l(U));
    rev(r(U));
    FlippedFlag[U] = 0;
  }
}

int LinkCutTree::sf(int U) { return U == r(FatherVertex[U]); }

bool LinkCutTree::isrt(int U) {
  return U != l(FatherVertex[U]) && U != r(FatherVertex[U]);
}

void LinkCutTree::rot(int U) {
  int V = FatherVertex[U], F = sf(U);
  bool Flag = isrt(V);
  if (!Flag)
    ChildVertex[FatherVertex[V]][sf(V)] = U;
  else if (FatherVertex[V])
    cutv(FatherVertex[V], V);
  ChildVertex[V][F] = ChildVertex[U][F ^ 1];
  FatherVertex[ChildVertex[V][F]] = V;
  FatherVertex[U] = FatherVertex[V];
  ChildVertex[U][F ^ 1] = V;
  FatherVertex[V] = U;
  pu(V);
  if (Flag) {
    pu(U);
    linkv(FatherVertex[U], U);
  }
}

void LinkCutTree::splay(int U) {
  int StackTop = 0;
  Stack[StackTop] = U;
  for (int V = U; !isrt(V); V = FatherVertex[V])
    Stack[++StackTop] = FatherVertex[V];
  for (; StackTop >= 0; pd(Stack[StackTop--]))
    ;
  for (; !isrt(U); rot(U))
    if (!isrt(FatherVertex[U]) && sf(FatherVertex[U]) == sf(U))
      rot(FatherVertex[U]);
}

void LinkCutTree::access(int U) {
  int W = U;
  for (int V = 0; U; U = FatherVertex[V = U]) {
    splay(U);
    linkv(U, r(U));
    cutv(U, V);
    r(U) = V;
    pu(U);
  }

  splay(W);
}
void LinkCutTree::makert(int U) {
  access(U);
  rev(U);
}

void LinkCutTree::join(int U, int V) {
  makert(U);
  access(V);
}

int LinkCutTree::findrt(int U) {
  access(U);
  for (; l(U); pd(U), U = l(U))
    ;
  splay(U);
  return U;
}

void LinkCutTree::link(int U, int V) {
  makert(U);
  if (findrt(V) == U)
    return;
  FatherVertex[U] = V;
  linkv(V, U);
  pu(V);
  access(V);
}

void LinkCutTree::cut(int U, int V) {
  join(U, V);
  if (l(V) != U || r(U))
    return;
  FatherVertex[U] = l(V) = 0;
  pu(V);
}

int LinkCutTree::get(int U, int F) {
  access(U);
  if (!Tag[U][F])
    return 0;
  while (G[U][F].empty()) {
    if (Tag[l(U)][F])
      U = l(U);
    else if (Tag[r(U)][F])
      U = r(U);
    else
      U = ChildV[F].first(U);
  }
  return U;
}
bool LinkCutTree::isconnected(int U, int V) { return findrt(U) == findrt(V); }
void LinkCutTree::ins(int F, int U, int V) {
  if (G[U][F].size() == 0)
    access(U);
  G[U][F].insert(V);
  pu(U);
  if (G[V][F].size() == 0)
    access(V);
  G[V][F].insert(U);
  pu(V);
}
void LinkCutTree::del(int F, int U, int V) {
  if (G[U][F].size() == 1)
    access(U);
  G[U][F].erase(V);
  pu(U);
  if (G[V][F].size() == 1)
    access(V);
  G[V][F].erase(U);
  pu(V);
}
} // namespace DynamicGraph
