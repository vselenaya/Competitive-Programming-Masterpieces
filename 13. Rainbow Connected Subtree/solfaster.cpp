/*
File name: solfaster.cpp
Tag: ACCEPTED
Author: tourist
Change time: Fri Nov 03 06:38:12 MSK 2023
*/


#include <bits/stdc++.h>

using namespace std;

#ifdef LOCAL
#include "algo/debug.h"
#else
#define debug(...) 42
#endif

template <typename T>
class graph {
 public:
  struct edge {
    int from;
    int to;
    T cost;
  };

  vector<edge> edges;
  vector<vector<int>> g;
  int n;

  graph(int _n) : n(_n) {
    g.resize(n);
  }

  virtual int add(int from, int to, T cost) = 0;
};

template <typename T>
class digraph : public graph<T> {
 public:
  using graph<T>::edges;
  using graph<T>::g;
  using graph<T>::n;

  digraph(int _n) : graph<T>(_n) {
  }

  int add(int from, int to, T cost = 1) {
    assert(0 <= from && from < n && 0 <= to && to < n);
    int id = (int) edges.size();
    g[from].push_back(id);
    edges.push_back({from, to, cost});
    return id;
  }

  digraph<T> reverse() const {
    digraph<T> rev(n);
    for (auto &e : edges) {
      rev.add(e.to, e.from, e.cost);
    }
    return rev;
  }
};

template <typename T>
vector<int> find_scc(const digraph<T> &g, int &cnt) {
  digraph<T> g_rev = g.reverse();
  vector<int> order;
  vector<bool> was(g.n, false);
  auto dfs1 = [&](auto& self, int v) -> void {
    was[v] = true;
    for (int id : g.g[v]) {
      auto &e = g.edges[id];
      int to = e.to;
      if (!was[to]) {
        self(self, to);
      }
    }
    order.push_back(v);
  };
  for (int i = 0; i < g.n; i++) {
    if (!was[i]) {
      dfs1(dfs1, i);
    }
  }
  vector<int> c(g.n, -1);
  auto dfs2 = [&](auto& self, int v) -> void {
    for (int id : g_rev.g[v]) {
      auto &e = g_rev.edges[id];
      int to = e.to;
      if (c[to] == -1) {
        c[to] = c[v];
        self(self, to);
      }
    }
  };
  cnt = 0;
  for (int id = g.n - 1; id >= 0; id--) {
    int i = order[id];
    if (c[i] != -1) {
      continue;
    }
    c[i] = cnt++;
    dfs2(dfs2, i);
  }
  return c;
  // c[i] <= c[j] for every edge i -> j
}

class twosat {
 public:
  digraph<int> g;
  int n;

  twosat(int _n) : g(digraph<int>(2 * _n)), n(_n) {
  }

  // (v[x] == value_x)
  inline void add(int x, int value_x) {
    assert(0 <= x && x < n);
    assert(0 <= value_x && value_x <= 1);
    g.add(2 * x + (value_x ^ 1), 2 * x + value_x);
  }
  
  // (v[x] == value_x || v[y] == value_y)
  inline void add(int x, int value_x, int y, int value_y) {
    assert(0 <= x && x < n && 0 <= y && y < n);
    assert(0 <= value_x && value_x <= 1 && 0 <= value_y && value_y <= 1);
    g.add(2 * x + (value_x ^ 1), 2 * y + value_y);
    g.add(2 * y + (value_y ^ 1), 2 * x + value_x);
  }

  inline vector<int> solve() {
    int cnt;
    vector<int> c = find_scc(g, cnt);
    vector<int> res(n);
    for (int i = 0; i < n; i++) {
      if (c[2 * i] == c[2 * i + 1]) {
        return vector<int>();
      }
      res[i] = (c[2 * i] < c[2 * i + 1]);
    }
    return res;
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  int tt;
  cin >> tt;
  while (tt--) {
    int n;
    cin >> n;
    n *= 2;
    vector<int> c(n);
    for (int i = 0; i < n; i++) {
      cin >> c[i];
      --c[i];
    }
    vector<vector<int>> g(n);
    for (int i = 0; i < n - 1; i++) {
      int x, y;
      cin >> x >> y;
      --x; --y;
      g[x].push_back(y);
      g[y].push_back(x);
    }
    vector<int> pv(n, -1);
    vector<int> sz(n);
    function<void(int)> Dfs = [&](int v) {
      sz[v] = 1;
      for (int u : g[v]) {
        if (u != pv[v]) {
          pv[u] = v;
          Dfs(u);
          sz[v] += sz[u];
        }
      }
    };
    pv[0] = -1;
    Dfs(0);
    int root = 0;
    while (true) {
      int nxt = -1;
      for (int u : g[root]) {
        if (u != pv[root] && 2 * sz[u] >= n) {
          nxt = u;
          break;
        }
      }
      if (nxt == -1) {
        break;
      }
      root = nxt;
    }
    fill(pv.begin(), pv.end(), -1);
    Dfs(root);
    vector<vector<int>> at(n / 2);
    for (int i = 0; i < n; i++) {
      at[c[i]].push_back(i);
    }
    twosat ts(n);
    for (int i = 0; i < n; i++) {
      if (i != root) {
        ts.add(pv[i], 1, i, 0);
      }
    }
    for (int i = 0; i < n / 2; i++) {
      assert(at[i].size() == 2);
      ts.add(at[i][0], 0, at[i][1], 0);
      ts.add(at[i][0], 1, at[i][1], 1);
    }
    auto res = ts.solve();
    if (res.empty()) {
      cout << -1 << '\n';
      continue;
    }
    vector<int> out;
    for (int i = 0; i < n; i++) {
      if (res[i] == 1) {
        out.push_back(i);
      }
    }
    assert((int) out.size() == n / 2);
    for (int i = 0; i < (int) out.size(); i++) {
      cout << out[i] + 1 << " \n"[i == (int) out.size() - 1];
    }
  }
  return 0;
}
