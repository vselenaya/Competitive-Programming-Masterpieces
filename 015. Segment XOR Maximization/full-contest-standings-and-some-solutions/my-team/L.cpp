#include <iostream>
#include <vector>

using namespace std;

int len(int x) {
    int l = 0;
    while (x > 0) {
        l += 1;
        x /= 2;
    }
    return l;
}

int main() {
    int n, q;
    cin >> n >> q;
    vector <int> a(n+1);
    vector <int> xr(n+1, 0);
    vector <vector <int>> num(32, vector <int> (n+1, 0));
    for (int i = 1; i <= n; ++i) {
        cin >> a[i];
        xr[i] = xr[i-1] ^ a[i];

        for (int j = 0; j < 32; ++j)
            num[j][i] = num[j][i-1];
        num[len(a[i])][i] += 1;
    }

    for (int i = 0; i < q; ++i) {
        int l, r;
        cin >> l >> r;

        int ans = xr[r] ^ xr[l-1];
        for (int j = 30; j >= 0; --j) {
            if (((1 << j) & ans) || (num[j+1][r] - num[j+1][l-1] == 0))
                continue;
            ans ^= ((1 << (j+1)) - 1);
        }

        cout << ans << '\n';
    }

    

}