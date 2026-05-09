#include <iostream>
#include <vector>
#include <bits/stdc++.h>

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

int per (char a) {
    return a - 'A' + 10;
}

int main() {
    string s;
    cin >> s;
    int n = s.size();
    int sum = 0;
    for (int i = 0; i < n-1; ++i) {
        sum += per(s[i]) * 16;
        sum %= 60;

    }
    int ans = 0;
    //cout << sum % 15 << ' ';
    sum += per(s[n-1]);
    sum %= 60;
    //cout << sum % 15 << ' ';
    vector<char> x;
    if (sum % 4 != 0) {
        x.push_back('C');
        sum += 12;
        sum += 15 * per(s[n-1]);
        sum %= 60;
        ans++;
    }
    sum %= 15;
    //cout << sum << ' ' << per('A') << ' ' << per('D') << '\n';
    if (sum == 0) {
        sum %= 15;
    }
    else if ( sum <= 5)
    {
        char t = 'A' + (5-sum);
        x.push_back(t);
    }
    else if (sum <= 10) {
        char t = 'A' + (10 - sum);
        x.push_back('A');
        x.push_back(t);
    }
    else {
        char t = 'A' + (15 - sum);
        x.push_back('A');
        x.push_back('A');
        x.push_back(t);
    }
    sort(x.begin(), x.end());
    if (x.size() == 0) 
        cout << "OK";
    else {
        for (int i = 0; i < x.size(); ++i)
        cout << x[i];
    }

}