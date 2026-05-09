
#include <iostream>
#include <cmath>
#include <vector> 
#include <string>
#include <algorithm>
#include <utility>
#include <bitset>
#include <queue>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <stdlib.h>
#include <cassert>

using namespace std;

#define int unsigned long long

template<typename T>
ostream& operator<<(ostream& s, vector<T>& a) {
    for (T& x : a) s << x << " ";
    s << endl;
    return s;
}

template<typename T1, typename T2>
ostream& operator<<(ostream& s, pair<T1,T2>& p) {
    s << "(" << p.first << ", " << p.second << ")";
    return s;
}

template<typename T1, typename T2>
ostream& operator<<(ostream& s, map<T1,T2>& m) {
    for (auto p : m) {
        s << p.first << " -> " << p.second << endl;
    }
    return s;
}

int reverse(int n) {
    int res = 0;
    for (int i = 0; i < 8; i++) {
        int byte = (n >> (8 * i)) % 256;
        //cout << byte << " ";
        res = (256 * res) + byte;
    }
    //cout << endl;
    return res;
}

vector<int> encode(vector<int> &a) {
    int n = a.size();
    a.resize(1000);
    vector<int> ans;
    bitset<17 * 64> flags = 0;
    for (int i = 0; i < 17; i++)
        flags.flip(i * 64);
    int j = 1;
    for (int i = 0; i < 1000; i++) {
        if (reverse(a[i]) > a[i])
            flags.flip(j);
        j++;
        if ((j % 64 == 0) || (j % 64 == 56)) j++;
        //cout << j % 64 << endl;
        assert(j != 56);
    }
    assert(flags.test(0));
    assert(!flags.test(56));
    for (int i = 0; i < 17; i++) {
        int res = 0;
        for (int j = 0; j < 64; j++) {
            res *= 2;
            if (flags.test(i * 64 + j)) res++;
        }
        ans.push_back(res);
    }
    for (int i = 0; i < n; i++) {
        ans.push_back(a[i]);
    }
    return ans;
}

vector<int> decode(vector<int> &a) {
    int n = a.size();
    vector<int> ans;
    for (int i = 0; i < 17; i++) {
        if (!(a[i] & (1ULL << 63))) {
            a[i] = reverse(a[i]);
        }
    }
    
    bitset<17 * 64> flags = 0;
    for (int i = 0; i < 17; i++) {
        for (int j = 0; j < 64; j++) {
            if (a[i] & (1ULL << (63 - j))) {
                flags.flip(64 * i + j);
            }
        }
    }
    
    int j = 1;
    for (int i = 17; i < n; i++) {
        if (!flags.test(j) && reverse(a[i]) > a[i]) {
            a[i] = reverse(a[i]);
        }
        if (flags.test(j) && reverse(a[i]) < a[i]) {
            a[i] = reverse(a[i]);
        }
        j++;
        if ((j % 64 == 0) || (j % 64 == 56)) j++;
        assert(j < 17 * 64);
        ans.push_back(a[i]);
    }
    return ans;
}

signed main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);
    cout.setf(ios::fixed);
    cout.precision(10);

    string s;
    cin >> s;
    if (s == "encode") {
        int n;
        cin >> n;
        vector<int> a(n);
        for (int i = 0; i < n; i++) cin >> a[i];
        vector<int> ans = encode(a);
        cout << ans.size() << endl;
        for (int x : ans) cout << x << " ";
        cout << endl;
    } else {
        int n;
        cin >> n;
        vector<int> a(n);
        for (int i = 0; i < n; i++) {
            cin >> a[i];
        }
        vector<int> ans = decode(a);
        for (int x : ans) cout << x << " ";
        cout << endl;
    }
   /*
    int n = 56;
    int test = 1;
    while (true) {
        cout << test << endl;
        vector<int> a(n);
        for (int i = 0; i < n; i++) a[i] = (rand() << 48) + (rand() << 32) + (rand() << 16) + rand();
        vector<int> encoded = encode(a), encoded_sh = encode(a);
        for (int i = 0; i < n; i++) {
            assert(encoded_sh[0] & (1ULL << 63));
            assert(!(encoded_sh[0] & 128));
            if (rand() % 2) {
                encoded_sh[i] = reverse(encoded_sh[i]);
            }
        }
        vector<int> decoded = decode(encoded_sh);
        for (int i = 0; i < n; i++) {
            if (decoded[i] != a[i]) {
                cout << "ERROR" << endl << a << encoded << encoded_sh << decoded;
                return 0;
            }
        }
        test++;
    }
    */
    return 0;
}
