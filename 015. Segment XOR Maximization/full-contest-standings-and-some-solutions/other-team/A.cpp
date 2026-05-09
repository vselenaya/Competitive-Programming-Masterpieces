#include <bits/stdc++.h>

using namespace std;

using ll = long long;

void solveTest() {
    vector<char> a(3);
    vector<ll> b(3);
    for (int i = 0; i < 3; ++i) cin >> a[i] >> b[i];
    bool ok = true;
    if (a[0] == a[1] && a[1] == a[2]) ok = false;
    else {
        char need = a[0] ^ a[1] ^ a[2];
        for (int i = 0; i < 3; ++i) {
            if (a[i] == need) {
                b[(i + 1) % 3] -= 1;
                b[(i + 2) % 3] -= 1;
            }
        }    
    }
    bool ok2 = false;
    ll sum = b[0] + b[1] + b[2];
    for (ll x : b) {
        if (x % 2 == 1 && x <= sum - x) {
            ok2 = true;
        }
    }
    cout << (ok ^ ok2) << "\n";
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int t; cin >> t;
  while (t--) solveTest();
  return 0;
}

