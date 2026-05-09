#include <iostream>
#include <vector>

using namespace std;

int solve(int n1, int n2, int n3) {
    int even = (n1 % 2 == 0) + (n2 % 2 == 0) + (n3 % 2 == 0);
    if (even == 3)
        return 0;
    if (even == 0)
        return 1;
    if (even == 2) {
        if (n1 % 2 == 1) {
            return n1 < n2 + n3;
        }
        if (n2 % 2 == 1) {
            return n2 < n1 + n3;
        }
        if (n3 % 2 == 1) {
            return n3 < n2 + n1;
        }
        return 1;
    }
        
    if (even == 1)
        return 1;
}

int main() {
    int t;
    cin >> t;

    for (int i = 0; i < t; ++i) {
        vector <int> a, b;
        for (int j = 0; j < 3; ++j) {
            char c;
            int x;
            cin >> c >> x;
            if (c == 'A')
                a.push_back(x);
            else
                b.push_back(x);
        }

        if (a.size() < b.size())
            std::swap(a, b);

        if (b.size() == 0) {
            cout << solve(a[0], a[1], a[2]) << '\n';
        } else {
            b.push_back(a[0] - 1);
            b.push_back(a[1] - 1);
            cout << (solve(b[0], b[1], b[2]) + 1) % 2 << '\n';
        }
    }
    
}