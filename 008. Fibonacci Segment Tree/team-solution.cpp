#define _CRT_SECURE_NO_WARNINGS
#define mp make_pair
#define rep(n, i) for (int i = 0; i < n; ++i)
#define per(n, i) for (int i = n - 1; i >= 0; --i)
#define allar(a, n) a, a + n
#define allv(a) a.begin(), a.end()
#define fastio ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

#include <unordered_map>
#include <immintrin.h>
#include <functional>
#include <algorithm>
#include <iostream>
#include <string.h>
#include <numeric>
#include <fstream>
#include <stdio.h>
#include <complex>
#include <iomanip>
#include <climits>
#include <bitset>
#include <math.h>
#include <string>
#include <random>
#include <vector>
#include <time.h>
#include <chrono>
#include <queue>
#include <deque>
#include <map>
#include <set>

using namespace std;

typedef long long ll;
typedef long double db;
typedef pair <int, int> pii;
typedef pair <ll, ll> pll;
typedef pair <db, db> pdd;
typedef complex <double> cmpl;

const ll MOD = (ll)1e9 + 7;
const ll MOD1 = 2286661337;
const ll MOD2 = 998244353;
const int INF = (int)1e9 + 7;
const db EPS = 1e-7;

const int N = (int)2e5 + 1;

int n, q;
ll a[N], F1_coef[N + 1], F2_coef[N + 1], F[N + 1], sum_F[N + 1];
ll tree_1[4 * N], tree_2[4 * N], add[4 * N];

void build(int v = 0, int vl = 0, int vr = n) {
    if (vl + 1 == vr) {
        tree_1[v] = a[vl];
        tree_2[v] = a[vl];
        return;
    }
    build(v * 2 + 1, vl, vl + vr >> 1);
    build(v * 2 + 2, vl + vr >> 1, vr);
    int F_idx = 1 + (vl + vr >> 1) - vl;
    tree_1[v] = (tree_1[v * 2 + 1] + tree_1[v * 2 + 2] * F1_coef[F_idx] % MOD + tree_2[v * 2 + 2] * F2_coef[F_idx] % MOD) % MOD;
    tree_2[v] = (tree_2[v * 2 + 1] + tree_1[v * 2 + 2] * F1_coef[F_idx + 1] % MOD + tree_2[v * 2 + 2] * F2_coef[F_idx + 1] % MOD) % MOD;
}

void push(int v, int vl, int vr) {
    if (add[v] == 0) {
        return;
    }
    tree_1[v] += sum_F[vr - vl] * add[v] % MOD;
    tree_1[v] %= MOD;
    tree_2[v] += (sum_F[vr - vl + 1] - F[1] + MOD) % MOD * add[v] % MOD;
    tree_2[v] %= MOD;
    if (vl + 1 != vr) {
        add[v * 2 + 1] = (add[v] + add[v * 2 + 1]) % MOD;
        add[v * 2 + 2] = (add[v] + add[v * 2 + 2]) % MOD;
    }
    add[v] = 0;
}

pll get_sum(int l, int r, int v = 0, int vl = 0, int vr = n) {
    push(v, vl, vr);
    if (vl >= r || vr <= l) {
        return { 0, 0 };
    }
    if (vl >= l && vr <= r) {
        return { tree_1[v], tree_2[v] };
    }
    pll res1 = get_sum(l, r, v * 2 + 1, vl, vl + vr >> 1);
    pll res2 = get_sum(l, r, v * 2 + 2, vl + vr >> 1, vr);
    int F_idx = max(0, (vl + vr >> 1) - max(vl, l)) + 1;
    pll res;
    res.first = (res1.first + res2.first * F1_coef[F_idx] % MOD + res2.second * F2_coef[F_idx] % MOD) % MOD;
    res.second = (res1.second + res2.first * F1_coef[F_idx + 1] % MOD + res2.second * F2_coef[F_idx + 1] % MOD) % MOD;
    return res;
}

void add_x(int l, int r, int x, int v = 0, int vl = 0, int vr = n) {
    push(v, vl, vr);
    if (vl >= r || vr <= l) {
        return;
    }
    if (vl >= l && vr <= r) {
        add[v] = (add[v] + x) % MOD;
        push(v, vl, vr);
        return;
    }
    add_x(l, r, x, v * 2 + 1, vl, vl + vr >> 1);
    add_x(l, r, x, v * 2 + 2, vl + vr >> 1, vr);
    int F_idx = 1 + (vl + vr >> 1) - vl;
    tree_1[v] = (tree_1[v * 2 + 1] + tree_1[v * 2 + 2] * F1_coef[F_idx] % MOD + tree_2[v * 2 + 2] * F2_coef[F_idx] % MOD) % MOD;
    tree_2[v] = (tree_2[v * 2 + 1] + tree_1[v * 2 + 2] * F1_coef[F_idx + 1] % MOD + tree_2[v * 2 + 2] * F2_coef[F_idx + 1] % MOD) % MOD;
}

signed main() {
#ifdef _DEBUG
    freopen("input.txt", "r", stdin); freopen("output.txt", "w", stdout);
#endif
    F1_coef[1] = 1;
    F2_coef[1] = 0;
    F1_coef[2] = 0;
    F2_coef[2] = 1;
    for (int i = 3; i <= N; ++i) {
        F1_coef[i] = (F1_coef[i - 1] + F1_coef[i - 2]) % MOD;
        F2_coef[i] = (F2_coef[i - 1] + F2_coef[i - 2]) % MOD;
    }
    F[1] = 1, F[2] = 1;
    for (int i = 3; i <= N; ++i) {
        F[i] = (F[i - 1] + F[i - 2]) % MOD;
    }
    for (int i = 1; i <= N; ++i) {
        sum_F[i] = (sum_F[i - 1] + F[i]) % MOD;
    }
    
    cin >> n >> q;
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }
    build();
    for (int i = 0; i < q; i++) {
        char c;
        int l, r;
        ll x;
        cin >> c;
        if (c == '+') {
            cin >> l >> r >> x;
            --l;
            add_x(l, r, x);
        } else {
            cin >> l >> r;
            --l;
            cout << get_sum(l, r).first << "\n";
        }
    }
    return 0;
}
