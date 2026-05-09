#include <iostream>

/* Какие-то кажется китайцы нашли формулу, поэтому решение может быть гораздо проще, чем в разборе:
        ans = 1 / (n+1) * ∑_{i=0}^{n} ( (-1)^i * C(n, i) * C(m-i+n, n) * C(m-i+n+1, n) ) 
*/
using namespace std;

static constexpr int MAX = 1'000'000 + 5;
static constexpr int MOD = 998'244'353;

// Глобальные массивы для кэширования факториалов
long long fact[MAX + 1];
long long inv_fact[MAX + 1];

// быстрое возведение в степень по модулю:
long long power(long long base, long long exp) {
    long long res = 1;
    base %= MOD;
    while (exp > 0) {
        if (exp % 2 == 1) res = (res * base) % MOD;
        base = (base * base) % MOD;
        exp /= 2;
    }
    return res;
}

long long inverse(long long n) {
    return power(n, MOD - 2);
}

// предподсчёт факториалов и их обратных
void precompute() {
    fact[0] = 1;
    for (int i = 1; i <= MAX; i++) {
        fact[i] = (fact[i - 1] * i) % MOD;
    }
    
    inv_fact[MAX] = inverse(fact[MAX]);  // обращаем только один раз! а потом вычисляем остальные факториалы
    for (int i = MAX - 1; i >= 0; i--) {
        inv_fact[i] = (inv_fact[i + 1] * (i + 1)) % MOD;
    }
}

// вычисляем биномиальный факториал C(n, k) % MOD
long long choose(int n, int k) {
    if (k < 0 || k > n) return 0;
    long long num = fact[n];
    long long den = (inv_fact[k] * inv_fact[n - k]) % MOD;
    return (num * den) % MOD;
}

// вычисляем C(n+k, k) % MOD
long long repeat_choose(int n, int k) {
    return choose(n + k, k);
}

int main() {
    int n, m;
    cin >> n >> m;
    precompute();

    long long ans = 0;
    for (int i = 0; i <= n; i++) {
        if (i > m) break;

        // Формула: C(n, i) * (-1)^i * C((m-i)+n, n) * C((m+1-i)+n, n)
        long long term = choose(n, i);
        term = (term * repeat_choose(m - i, n)) % MOD;
        term = (term * repeat_choose(m + 1 - i, n)) % MOD;

        if (i % 2 == 1) {
            ans = (ans - term + MOD) % MOD;
        } else {
            ans = (ans + term) % MOD;
        }
    }

    cout << (ans * inverse(n + 1)) % MOD << endl;

    return 0;
}
