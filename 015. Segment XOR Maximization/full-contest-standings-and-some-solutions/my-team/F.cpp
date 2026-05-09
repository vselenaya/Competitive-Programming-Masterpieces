#include <iostream>
#include <vector>

using namespace std;

int gcd(int a, int b) {
    if (a < 0) {
        a *= -1;
    }
    if (b < 0) {
        b *= -1;
    }
    if (a < b) {
        std::swap(a, b);
    }
    while (b > 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

int del(int64_t n) {
    int ans = 0;
    for (int64_t i = 1; i * i <= n; ++i) {
        if (n % i == 0)
            ans += 2;
        if ( i * i == n)
        ans--;
    }
    return ans;
}


int odd_del(int64_t n) {
    int ans = 0;
    for (int64_t i = 1; i * i <= n; ++i) {
        if (n % i != 0)
            continue;
        if (i % 2 != 0)
            ans++;
        if (n / i != i && (n / i) % 2 != 0)
            ans++;
    }
    return ans;
}

int main() {
    int64_t n;
    cin >> n;
    int ans = odd_del(n) - 1;
    if (n % 4 == 0)
        ans += del(n / 4);
    for (int64_t i = 4; i * i - 1<= n; i+=2) {
        if (n % (i * i - 1) == 0)
            ans++;
    }
    for (int64_t i = 2; 2 * i * (i+1) <= n; ++i) {
        if (n % (2 * i * (i+1)) == 0) ans++;
    }
    cout << ans << '\n';
}