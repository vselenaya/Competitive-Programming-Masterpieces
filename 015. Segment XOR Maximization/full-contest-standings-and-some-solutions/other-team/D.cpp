#include <bits/stdc++.h>

using namespace std;

using ll = long long;

const int MOD = 60;

void brute(std::string& cur, int len, int pref, int n, int rem, int pw) {
    if ((int)cur.size() == len) {
        int suff = len - pref;
        int prefRem = 0;
        int suffRem = 0;
        for (int i = 0; i < pref; ++i) {
            int curD = cur[i] - 'A' + 10;
            prefRem = (prefRem * 16 + curD) % MOD;
        }
        int suffPw = 1;
        for (int i = 0; i < suff; ++i) {
            int curD = cur[i + pref] - 'A' + 10;
            suffRem = (suffRem * 16 + curD) % MOD;
            suffPw = (suffPw * 16) % MOD;
        }

        int totalRem = suffRem;
        totalRem = (totalRem + rem * suffPw) % MOD;
        totalRem = (totalRem + prefRem * suffPw * pw) % MOD;
        if (totalRem == 0) {
            std::sort(cur.begin(), cur.end());
            std::cout << cur << '\n';
            exit(0);
        }

        return;
    }

    for (char c = 'A'; c <= 'F'; ++c) {
        cur.push_back(c);
        brute(cur, len, pref, n, rem, pw);
        cur.pop_back();
    }
}

void solveTest() {
    int n, m, a;
    std::cin >> n >> m >> a;

    if (a == 0) {
        std::cout << std::max(0, m - 1) << '\n';
        return;
    }
    if (n == 0) {
        std::cout << m - 1 << '\n';
        return;
    }
    if (m == 0) {
        std::cout << "0\n";
        return;
    }
    if (a == 1 || n == 1) {
        std::cout << m << '\n';
        return;
    }

    int x = std::min({m, n - 1, a / 2});
    int ans = 0;
    ans += 2 * x;
    m -= x;
    n -= x + 1;
    a -= 2 * x;
    if (a > 0 && m > 0) {
        ++ans;
        --a;
        --m;
    }
    ans += m;

    std::cout << ans << '\n';
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int t; cin >> t;
    // int t = 1;
  while (t--) solveTest();
  return 0;
}

