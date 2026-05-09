#include <bits/stdc++.h>

using namespace std;

using ll = long long;
using I = __int128;

void solveTest() {
    ll n; cin >> n;
    vector<ll> divs;
    divs.reserve(1e5);
    for (ll d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            divs.push_back(d);
            if (d * d != n) {
                divs.push_back(n / d);
            }
        }
    }
    set<pair<I, I>> st;

    auto work = [&](ll a, ll b, ll k) {
        if (a == 0 || b == 0 || a == b || k <= 0) {
            return;
        }
        if (a < b) {
            swap(a, b);
        }
        
        st.insert(minmax(I(k) * (I(a) * a - I(b) * b), I(k) * 2 * a * b));
    };

    for (ll x : divs) {
        for (ll y : divs) {
            if (x < y) {
                continue;
            }

            {
                //  x = a, y = b
                bool ok = false;
                if (x % y == 0) {
                    ok = true;
                } else {
                    if (x != y && (x + y) % (x - y) == 0) {
                        ok = true;
                    }
                }
                if (!ok) continue;

                ll a = x;
                ll b = y;
                ll curN = n;
                bool bad = false;
                if (curN % 2) {
                    bad = true;
                } else {
                    curN /= 2;
                    if (curN % a) {
                        bad = true;
                    } else {
                        curN /= a;
                        if (curN % b) {
                            bad = true;
                        }
                    }
                }
                if (!bad) {
                    ll k = n / 2 / a / b;
                    work(a, b, k);
                }
            }

            {
                //  x = a + b, y = a - b;
                if (x % 2 != y % 2) {
                    continue;
                }
                ll a = (x + y) / 2;
                ll b = (x - y) / 2;
                
                bool ok = false;
                if (x % y == 0) {
                    ok = true;
                } else {
                    if (b != 0 && a % b == 0) {
                        ok = true;
                    }   
                }
                if (!ok) continue;
                ll curN = n;
                bool bad = false;
                if (curN % x) {
                    bad = true;
                } else {
                    curN /= x;
                    if (curN % y) {
                        bad = true;
                    }
                }
                if (!bad) {
                    ll k = n / x / y;
                    work(a, b, k);
                }
            }
        }
    }

    // for (auto[a, b] : st) cerr << ll(a) << " " << ll(b) << "\n";
    cout << st.size() << "\n";
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int t = 1;
  while (t--) solveTest();
  return 0;
}
