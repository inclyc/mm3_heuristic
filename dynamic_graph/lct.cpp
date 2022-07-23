#include "lct.hpp"
#include <unordered_map>
#include <unordered_set>

namespace DynamicGraph {

#define l(u) ch[u][0]
#define r(u) ch[u][1]

void LCT::init(int n) {
  for (int i = 0; i < 2; ++i) {
    chv[i].init(n);
    for (int u = 1; u <= n; ++u)
      G[u][i].clear();
  }
  tp = 0;
  for (int u = 0; u <= n; ++u) {
    sizv[u] = 0;
    sta[u] = 0;
    fa[u] = l(u) = r(u) = tag[u][0] = tag[u][1] = flp[u] = sizv[u] = 0,
    siz[u] = 1;
  }
  siz[0] = 0;
}

void LCT::free() {
  chv[0].free();
  chv[1].free();
}

void LCT::linkv(int u, int v) {
  if (!u || !v)
    return;
  sizv[u] += siz[v];
  for (int i = 0; i < 2; ++i)
    if (tag[v][i])
      chv[i].link(u, v);
}
void LCT::cutv(int u, int v) {
  if (!u || !v)
    return;
  sizv[u] -= siz[v];
  for (int i = 0; i < 2; ++i)
    if (tag[v][i])
      chv[i].cut(u, v);
}
void LCT::rev(int u) {
  flp[u] ^= 1;
  std::swap(l(u), r(u));
}

void LCT::pu(int u) {
  if (!u)
    return;
  for (int i = 0; i < 2; ++i)
    tag[u][i] =
        tag[l(u)][i] | tag[r(u)][i] | !G[u][i].empty() | !chv[i].empty(u);
  siz[u] = siz[l(u)] + siz[r(u)] + 1 + sizv[u];
}

void LCT::pd(int u) {
  if (flp[u])
    rev(l(u)), rev(r(u)), flp[u] = 0;
}

int LCT::sf(int u) { return u == r(fa[u]); }

bool LCT::isrt(int u) { return u != l(fa[u]) && u != r(fa[u]); }

void LCT::rot(int u) {
  int v = fa[u], f = sf(u);
  bool flag = isrt(v);
  if (!flag)
    ch[fa[v]][sf(v)] = u;
  else if (fa[v])
    cutv(fa[v], v);
  ch[v][f] = ch[u][f ^ 1], fa[ch[v][f]] = v;
  fa[u] = fa[v], ch[u][f ^ 1] = v, fa[v] = u, pu(v);
  if (flag)
    pu(u), linkv(fa[u], u);
}

void LCT::splay(int u) {
  sta[tp = 0] = u;
  for (int v = u; !isrt(v); v = fa[v])
    sta[++tp] = fa[v];
  for (; ~tp; pd(sta[tp--]))
    ;
  for (; !isrt(u); rot(u))
    if (!isrt(fa[u]) && sf(fa[u]) == sf(u))
      rot(fa[u]);
}

void LCT::access(int u) {
  int w = u;
  for (int v = 0; u; u = fa[v = u])
    splay(u), linkv(u, r(u)), cutv(u, v), r(u) = v, pu(u);
  splay(w);
}
void LCT::makert(int u) {
  access(u);
  rev(u);
}

void LCT::join(int u, int v) {
  makert(u);
  access(v);
}

int LCT::findrt(int u) {
  access(u);
  for (; l(u); pd(u), u = l(u))
    ;
  splay(u);
  return u;
}

void LCT::link(int u, int v) {
  makert(u);
  if (findrt(v) == u)
    return;
  fa[u] = v, linkv(v, u), pu(v), access(v);
}

void LCT::cut(int u, int v) {
  join(u, v);
  if (l(v) != u || r(u))
    return;
  fa[u] = l(v) = 0, pu(v);
}

int LCT::get(int u, int f) {
  access(u);
  if (!tag[u][f])
    return 0;
  while (G[u][f].empty()) {
    if (tag[l(u)][f])
      u = l(u);
    else if (tag[r(u)][f])
      u = r(u);
    else
      u = chv[f].first(u);
  }
  return u;
}
bool LCT::isconnected(int u, int v) { return findrt(u) == findrt(v); }
void LCT::ins(int f, int u, int v) {
  if (G[u][f].size() == 0)
    access(u);
  G[u][f].insert(v), pu(u);
  if (G[v][f].size() == 0)
    access(v);
  G[v][f].insert(u), pu(v);
}
void LCT::del(int f, int u, int v) {
  if (G[u][f].size() == 1)
    access(u);
  G[u][f].erase(v), pu(u);
  if (G[v][f].size() == 1)
    access(v);
  G[v][f].erase(u), pu(v);
}
} // namespace DynamicGraph