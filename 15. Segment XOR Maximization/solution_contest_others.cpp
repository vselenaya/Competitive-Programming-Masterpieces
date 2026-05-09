#include <bits/stdc++.h>

using namespace std;

using ll = long long;

void solveTest() {
    int n, q;
    cin >> n >> q;
    vector<int> a(n);
    for (int& x : a) cin >> x;
    vector<int> xr(n + 1);
    int const B = 30;
    vector<array<int, 30>> cnt(n + 1);
    for (int i = 0; i < n; ++i) {
        xr[i + 1] = xr[i] ^ a[i];
        cnt[i + 1] = cnt[i];
        if (a[i] > 0) {
            int bit = 31 - __builtin_clz(a[i]);
            cnt[i + 1][bit]++;
        }
    }
    while (q--) {
        int l, r;
        cin >> l >> r;
        --l;
        int x = xr[r] ^ xr[l];
        auto br = cnt[r];
        auto bl = cnt[l];
        for (int i = B - 1; i >= 0; --i) {
            if ((x >> i & 1) == 0 && br[i] - bl[i] > 0) {
                x ^= ((1 << (i + 1)) - 1);
            }
        }
        cout << x << "\n";
    }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int t = 1;
  while (t--) solveTest();
  return 0;
}

