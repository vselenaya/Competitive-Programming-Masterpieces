#include <bits/stdc++.h>

using namespace std;

using ll = long long;

int const MOD = 998244353;

int add(int x, int y) {
    x += y;
    if (x >= MOD) {
        x -= MOD;
    }
    return x;
}

void add_self(int& x, int y) {
    x = add(x, y);
}

int sub(int x, int y) {
    x -= y;
    if (x < 0) {
        x += MOD;
    }
    return x;
}

int mul(ll x, int y) {
    return int(x * y % MOD);
}

int power(int x, int d) {
    int res = 1;
    while (d) {
        if (d & 1) {
            res = mul(res, x);
        }
        x = mul(x, x);
        d /= 2;
    }
    return res;
}

int inv(int x) {
    return power(x, MOD - 2);
}

int const N = 501;
int dp[N][N][N];
int temp_dp[N][N];
int temp_cnt[N];

void solveTest() {
    int n;
    cin >> n;
    vector<vector<int>> graph(n);
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        cin >> u >> v;
        --u, --v;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }
    vector<int> sz(n);
    auto dfs = [&](auto self, int v, int p = -1) -> void {
        sz[v] = 1;
        for (int u : graph[v]) {
            if (u == p) {
                continue;
            }
            self(self, u, v);
            sz[v] += sz[u];
        }
        dp[v][0][0] = dp[v][1][0] = dp[v][1][1] = 0;
        dp[v][0][1] = 1;
        int cur_size = 1;
        for (int u : graph[v]) {
            if (u == p) {
                continue;
            }
            int new_size = cur_size + sz[u];
            for (int new_mex = 0; new_mex <= new_size; ++new_mex) {
                for (int new_cnt = 0; new_cnt <= new_size; ++new_cnt) {
                    temp_dp[new_mex][new_cnt] = 0;
                }
            }

            // for (int mex_u = 0; mex_u <= sz[u]; ++mex_u) {
            //     for (int cnt_u = 0; cnt_u <= sz[u]; ++cnt_u) {
            //         int val = dp[u][mex_u][cnt_u];
            //         if (val == 0) {
            //             continue;
            //         }
            //         for (int old_mex = 0; old_mex <= cur_size; ++old_mex) {
            //             for (int old_cnt = 0; old_cnt <= cur_size; ++old_cnt) {
            //                 if (mex_u > old_mex) {
            //                     add_self(temp_dp[max(mex_u, old_mex)][cnt_u + old_cnt], mul(dp[v][old_mex][old_cnt], val));
            //                 }
            //             }
            //         }
            //     }
            // }

            //  Case 1. mex_u <= old_mex
            for (int cnt = 0; cnt <= sz[u]; ++cnt) {
                temp_cnt[cnt] = 0;
            }
            for (int old_mex = 0; old_mex <= cur_size; ++old_mex) {
                //  activate old_mex
                if (old_mex <= sz[u]) {
                    for (int cnt_u = 0; cnt_u <= sz[u] && cnt_u <= sz[u]; ++cnt_u) {
                        add_self(temp_cnt[cnt_u], dp[u][old_mex][cnt_u]);
                    }
                }
                for (int cnt_u = 0; cnt_u <= sz[u]; ++cnt_u) {
                    int val = temp_cnt[cnt_u];
                    if (val == 0) {
                        continue;
                    }
                    for (int old_cnt = 0; old_cnt <= cur_size; ++old_cnt) {
                        add_self(temp_dp[old_mex][cnt_u + old_cnt], mul(dp[v][old_mex][old_cnt], val));
                    }
                }
            }

            //  Case 2. mex_u > old_mex
            for (int cnt = 0; cnt <= cur_size; ++cnt) {
                temp_cnt[cnt] = 0;
            }
            for (int mex_u = 0; mex_u <= sz[u]; ++mex_u) {
                for (int old_cnt = 0; old_cnt <= cur_size; ++old_cnt) {
                    int val = temp_cnt[old_cnt];
                    if (val == 0) {
                        continue;
                    }
                    for (int cnt_u = 0; cnt_u <= sz[u]; ++cnt_u) {
                        add_self(temp_dp[mex_u][cnt_u + old_cnt], mul(dp[u][mex_u][cnt_u], val));
                    }
                }

                if (mex_u <= cur_size) {
                    for (int old_cnt = 0; old_cnt <= cur_size; ++old_cnt) {
                        add_self(temp_cnt[old_cnt], dp[v][mex_u][old_cnt]);
                    }
                }
            }

            for (int new_mex = 0; new_mex <= new_size; ++new_mex) {
                for (int new_cnt = 0; new_cnt <= new_size; ++new_cnt) {
                    dp[v][new_mex][new_cnt] = temp_dp[new_mex][new_cnt];
                }
            }

            cur_size = new_size;
        }

        for (int new_mex = 0; new_mex <= cur_size; ++new_mex) {
            for (int new_cnt = 0; new_cnt <= cur_size; ++new_cnt) {
                temp_dp[new_mex][new_cnt] = 0;
            }
        }

        for (int old_mex = 0; old_mex <= cur_size; ++old_mex) {
            for (int old_cnt = 0; old_cnt <= cur_size; ++old_cnt) {
                add_self(temp_dp[old_mex][old_cnt], dp[v][old_mex][old_cnt]);
                dp[v][old_mex][old_cnt] = temp_dp[old_mex][old_cnt];
                if (old_mex + 1 <= cur_size && old_cnt >= 1) {
                    add_self(temp_dp[old_mex + 1][old_cnt - 1], temp_dp[old_mex][old_cnt]);
                }
            }
        }
    };
    dfs(dfs, 0);

    int ans = 0;
    for (int mex = 0; mex <= n; ++mex) {
        for (int cnt = 0; cnt <= n; ++cnt) {
            add_self(ans, dp[0][mex][cnt]);
        }
    }

    cout << ans << "\n";
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  int t; cin >> t;
  while (t--) solveTest();
  return 0;
}

