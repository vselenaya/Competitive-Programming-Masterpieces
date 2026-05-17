#define _CRT_SECURE_NO_WARNINGS

#include <set>
#include <map>
#include <cfenv>
#include <queue>
#include <math.h>
#include <vector>
#include <string>
#include <random>
#include <numeric>
#include <iomanip>
#include <cassert>
#include <complex>
#include <iostream>
#include <iterator>
#include <memory.h>
#include <algorithm>

using namespace std;

typedef long long ll;
typedef long double db;
typedef complex <db> cmpl;
typedef pair <int, int> pii;

const ll MOD1 = 998244353;
const ll MOD2 = (int)1e9 + 7;
const db EPS = 1e-9;
const int INF = (int)1e9;

const int N = (int)1e6 + 1;

int n, a[N], ans;
bool in_a[N];
vector <int> divisors[N];

int main() {
#ifdef _DEBUG
    freopen("input.txt", "r", stdin); freopen("output.txt", "w", stdout);
#endif
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        scanf("%d", &a[i]);
        in_a[a[i]] = true;
    }
    for (int d = 2; d < N; ++d) {
        for (int i = d; i < N; i += d) {
            divisors[i].push_back(d);
        }
    }
    for (int i = 0; i < n; ++i) {
        for (int d1 : divisors[a[i]]) {
            int d2 = a[i] / d1;
            int delta = abs(d1 - d2);
            if (delta % 2 == 0 && in_a[delta / 2]) {
                ++ans;
            }
        }
    }
    printf("%d", ans);
    return 0;
}
