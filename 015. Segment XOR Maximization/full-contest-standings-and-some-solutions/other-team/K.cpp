#include <bits/stdc++.h>

using namespace std;

using ll = long long;

int const INF = int(1e9);

vector<int> get_dp(vector<int> a) {
    int n = int(a.size());
    
    vector<int> dp(n + 1, INF);
    dp[0] = -INF;

    vector<int> shit_dp(n);
    for (int i = 0; i < n; ++i) {
        if (a[i] == 0) {
            continue;
        }
        auto it = lower_bound(dp.begin(), dp.end(), a[i]);
        int len = it - dp.begin();
        *it = a[i];
        shit_dp[i] = len;
    }

    return shit_dp;
}

void solveTest(int n) {
    vector<int> a(n);
    for (int& x : a) cin >> x;
    a.insert(a.begin(), -n - 100);
    a.insert(a.end(), n + 100);
    n += 2;

    auto dp = get_dp(a);

    int len = *max_element(dp.begin(), dp.end());

    auto a_rev = a;
    reverse(a_rev.begin(), a_rev.end());
    for (int i = 0; i < n; ++i) {
        if (a_rev[i] == 0) {
            continue;
        }
        a_rev[i] = -a_rev[i];
    }
    auto dp_rev = get_dp(a_rev);
    reverse(dp_rev.begin(), dp_rev.end());

    vector<int> nxt_zero(n + 1, n);
    for (int i = n - 1; i >= 0; --i) {
        if (a[i] == 0) {
            nxt_zero[i] = i;
        } else {
            nxt_zero[i] = nxt_zero[i + 1];
        }
    }

    vector<vector<int>> by_level(n + 1);
    for (int i = 0; i < n; ++i) {
        if (a[i] == 0) {
            continue;
        }
        if (dp[i] + dp_rev[i] - 1 == len) {
            by_level[dp[i]].push_back(i);
        }
    }

    vector<int> shit(n + 3);

    for (int level = 0; level + 1 <= n; ++level) {
        auto& v1 = by_level[level];
        auto& v2 = by_level[level + 1];
        if (v1.empty() || v2.empty()) {
            continue;
        }
        vector<int> sorted_pos;
        merge(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(sorted_pos));
        int cnt = int(sorted_pos.size());
        vector<pair<int, int>> answers;
        for (int i = cnt - 1; i >= 0; --i) {
            int pos = sorted_pos[i];
            if (dp[pos] == level) {
                int nzero = nxt_zero[pos];
                if (nzero == n) {
                    continue;
                }

                int cur_sz = int(answers.size());
                int left = -1;
                int right = cur_sz;

                while (left + 1 != right) {
                    int mid = (left + right) / 2;
                    if (answers[mid].first >= nzero) {
                        left = mid;
                    } else {
                        right = mid;
                    }
                }

                if (left >= 0) {
                    int from = a[pos] + 1;
                    int to = answers[left].second - 1;
                    from = max(1, from);
                    to = min(to, n - 2);
                    if (from <= to) {
                        shit[from] += 1;
                        shit[to + 1] -= 1;
                    }
                }
            } else {
                assert(dp[pos] == level + 1);
                answers.emplace_back(pos, a[pos]);
                if (answers.size() >= 2) {
                    answers.back().second = max(answers.back().second, answers.end()[-2].second);
                }
            }
        }
    }

    // cout << "len is: " << len - 2 << "\n";
    int cur_sum = shit[0];
    ll answer = 0;
    for (int i = 1; i <= n - 2; ++i) {
        cur_sum += shit[i];
        if (cur_sum > 0) {
            // cout << i << ": " << len - 1 << "\n";
            answer += ll(i) * (len - 1);
        } else {
            // cout << i << ": " << len - 2 << "\n";
            answer += ll(i) * (len - 2);
        }
    }

    cout << answer << "\n";
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  
  int n;
  while (true) {
    cin >> n;
    if (n == 0) break;
    solveTest(n);
  }
//   int t = 1;
//   while (t--) solveTest();
  return 0;
}

