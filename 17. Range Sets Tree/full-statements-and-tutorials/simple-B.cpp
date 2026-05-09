#include <bits/stdc++.h>

/* Решение через бинарный поиск (гораздо проще, чем в разборе) */

using namespace std;

#define int long long

bool can_m(int m, int n, int k, vector<int> &x) {
    __int128_t s = 0;
    for (int i = 0; i < n; ++i) {
        s += min(x[i], m);  // сколько можем взять этого цвета суммарно за все m ходов
    }
    return s >= __int128(m) * k;  // m * k = суммарно шаров взяли за m ходов
}

signed main() {
    int n, k;
    cin >> n >> k;

    vector<int> x(n);
    for (int i = 0; i < n; ++i)
        cin >> x[i];

    int l = 0, r = 1e18;
    while (r - l > 1) {
        int m = (l + r) / 2;  // хотим сделать m шагов
        if (can_m(m, n, k, x))
            l = m;
        else
            r = m;
    }

    cout << l << endl;
}
