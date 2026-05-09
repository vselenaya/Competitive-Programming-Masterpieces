#include <bits/stdc++.h>

using namespace std;

using ll = long long;

ll LINF = (ll)2e18 + 100;

void solveTest() {
    int n, a, b, t1, t2, t3, t4, t5, t6;
    std::cin >> n >> a >> b >> t1 >> t2 >> t3 >> t4 >> t5 >> t6;

    int d = b - a;
    if (d < 0) {
        d += n;
    }

    ll ans = LINF;
    ans = std::min(ans, 1ll * d * t1);
    ans = std::min(ans, 1ll * (n - d) * t2);
    ans = std::min(ans, 1ll * d * t4);
    ans = std::min(ans, 1ll * (n - d) * t3);
    ans = std::min(ans, 1ll * (d / 2) * t5 + 1ll * (d % 2) * std::min({t1, t4, t2 + t5, t3 + t5}));
    ans = std::min(ans, 1ll * ((n - d) / 2) * t6 + 1ll * ((n - d) % 2) * std::min({t2, t3, t1 + t6, t4 + t6}));

    std::cout << ans << '\n';
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int t = 1;
  std::cin >> t;
  while (t--) solveTest();
  return 0;
}

