/*
File name: 230409226_Hello_zoka_ok.cpp
Tag: ACCEPTED
Author: codeforces
Change time: Fri Nov 03 03:41:03 MSK 2023
*/

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <cmath>

#define sqr(x) ((x) * (x))
#define endl '\n'
//#define int ll

using namespace std;

typedef long long ll;
typedef long double ld;
typedef pair<int, int> pii;
typedef vector<int> vi;
typedef vector<pii> vii;
typedef vector<vi> vvi;
typedef vector<char> vc;

void dfs(int v, vvi& a, vi& sz, int p = -1) {
    sz[v] = 1;
    for (int u : a[v]) {
        if (u == p) continue;
        dfs(u, a, sz, v);
        sz[v] += sz[u];
    }
}

void dfs_all(int v, vvi& a, vi& vs, int p) {
    vs.push_back(v);
    for (int u : a[v]) {
        if (u == p) continue;
        dfs_all(u, a, vs, v);
    }
}

int find_centr(int v, vvi& a, vi& sz, int n, int p = -1) {
    int u1 = -1;
    for (int u : a[v]) {
        if (u == p) continue;
        if (sz[u] > n) {
            u1 = u;
            break;
        }
    }
    if (u1 == -1) return v;
    return find_centr(u1, a, sz, n, v);
}

void dfs_par(int v, vvi& a, vi& par, int p = -1) {
    par[v] = p;
    for (int u : a[v]) {
        if (u == p) continue;
        dfs_par(u, a, par, v);
    }
}

void dfs_ts(int v, vvi& g, vc& used, vi& ts) {
    used[v] = 1;
    for (int u : g[v]) {
        if (!used[u]) {
            dfs_ts(u, g, used, ts);
        }
    }
    ts.push_back(v);
}

void dfs_col(int v, vvi& g, vi& c, int col) {
    c[v] = col;
    for (int u : g[v]) {
        if (c[u] == -1) {
            dfs_col(u, g, c, col);
        }
    }
}

vi find_comp(vvi& g) {
    int n = (int)g.size();
    vi ts;
    vc used(n);
    vvi h(n);
    for (int v = 0; v < n; ++v) {
        for (int u : g[v]) {
            h[u].push_back(v);
        }
    }
    for (int i = 0; i < n; ++i) {
        if (!used[i]) {
            dfs_ts(i, g, used, ts);
        }
    }
    reverse(ts.begin(), ts.end());
    vi c(n, -1);
    int cnt = 0;
    for (int v : ts) {
        if (c[v] == -1) {
            dfs_col(v, h, c, cnt++);
        }
    }
    return c;
}

void solution() {
    int n;
    cin >> n;
    vvi a(2 * n);
    vi col(2 * n);
    vvi c(n);
    for (int i = 0; i < 2 * n; ++i) {
        cin >> col[i];
        --col[i];
        c[col[i]].push_back(i);
    }
    for (int i = 0; i < 2 * n - 1; ++i) {
        int u, v;
        cin >> u >> v;
        --u; --v;
        a[u].push_back(v);
        a[v].push_back(u);
    }
    vi sz(2 * n);
    dfs(0, a, sz);
    int v0 = find_centr(0, a, sz, n);
    dfs(v0, a, sz);
    for (int u : a[v0]) {
        if (sz[u] == n) {
            vi vs;
            dfs_all(u, a, vs, v0);
            set<int> cs;
            for (int i : vs) {
                cs.insert(col[i]);
            }
            if (cs.size() == n) {
                for (int i : vs) cout << i + 1 << ' ';
                cout << endl;
                return;
            }
        }
    }
    vi duo(2 * n);
    for (int i = 0; i < n; ++i) {
        duo[c[i][0]] = c[i][1];
        duo[c[i][1]] = c[i][0];
    }
    vi p(2 * n);
    dfs_par(v0, a, p);
    vvi g(2 * n);
    for (int i = 0; i < n; ++i) {
        int u = c[i][0], v = c[i][1];
        if (p[u] != -1) {
            g[u].push_back(p[u]);
            g[duo[p[u]]].push_back(v);
        }
        if (p[v] != -1) {
            g[v].push_back(p[v]);
            g[duo[p[v]]].push_back(u);
        }
    }
    vi comp = find_comp(g);
    vi ans;
    for (int i = 0; i < n; ++i) {
        int u = c[i][0], v = c[i][1];
        if (comp[u] == comp[v]) {
            cout << -1;
            cout << endl;
            return;
        }
        if (comp[u] > comp[v]) {
            ans.push_back(u);
        } else {
            ans.push_back(v);
        }
    }
    for (int v : ans) cout << v + 1 << ' ';
    cout << endl;
}

signed main() {
#ifdef LC
    freopen("/Users/yurij/Documents/JUB/ICPC/semester3/SeminSPB23/input.txt", "r", stdin);
    freopen("/Users/yurij/Documents/JUB/ICPC/semester3/SeminSPB23/output.txt", "w", stdout);
#endif
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    int t = 1;
    cin >> t;
    for (int i = 0; i < t; ++i) solution();
}
