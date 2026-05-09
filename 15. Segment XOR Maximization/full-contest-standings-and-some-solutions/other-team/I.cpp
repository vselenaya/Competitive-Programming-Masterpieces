#include <bits/stdc++.h>

using namespace std;

using ll = long long;

int ask(int x) {
    cout << "? " << x << endl;
    int resp; cin >> resp; return resp;
}

void solveTest() {
    int const N = 1986;
    vector<bool> is_prime(N + 1, true);
    is_prime[0] = is_prime[1] = false;
    vector<int> primes;
    for (int p = 2; p <= N; ++p) {
        if (is_prime[p]) {
            primes.push_back(p);
            for (int q = p + p; q <= N; q += p) {
                is_prime[q] = false;
            }
        }
    }
    int cur_prod = 1;
    ask(1);
    for (int p : primes) {
        int cur_power = 1;
        while (cur_prod * p <= N && ask(cur_power * p) == 0) {
            cur_prod *= p;
            cur_power *= p;
        }
    }
    cout << "! " << cur_prod << endl;
}

int main() {
    
  int t; cin >> t;
  while (t--) solveTest();
  return 0;
}

