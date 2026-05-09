#include <bits/stdc++.h>

using namespace std;

using ll = long long;

const int MOD = 60;

std::vector<std::string> fuckingShit;

void brute(std::string& cur, int len, int pref, int n, int rem, int pw) {
    if ((int)cur.size() == len) {
        int suff = len - pref;
        int prefRem = 0;
        int suffRem = 0;
        for (int i = 0; i < pref; ++i) {
            int curD = cur[i] - 'A' + 10;
            prefRem = (prefRem * 16 + curD) % MOD;
        }
        int suffPw = 1;
        for (int i = 0; i < suff; ++i) {
            int curD = cur[i + pref] - 'A' + 10;
            suffRem = (suffRem * 16 + curD) % MOD;
            suffPw = (suffPw * 16) % MOD;
        }

        int totalRem = suffRem;
        totalRem = (totalRem + rem * suffPw) % MOD;
        totalRem = (totalRem + prefRem * suffPw * pw) % MOD;
        if (totalRem == 0) {
            fuckingShit.push_back(cur);
        }

        return;
    }

    for (char c = 'A'; c <= 'F'; ++c) {
        cur.push_back(c);
        brute(cur, len, pref, n, rem, pw);
        cur.pop_back();
    }
}

void solveTest() {
    std::string s;
    std::cin >> s;

    int n = (int)s.size();
    int rem = 0;
    int pw = 1;
    for (auto c : s) {
        int cur = c - 'A' + 10;
        rem = (rem * 16 + cur) % MOD;
        pw = (pw * 16) % MOD;
    }

    if (rem == 0) {
        std::cout << "OK\n";
        return;
    }

    for (int len = 1; ; ++len) {
        for (int pref = 0; pref <= len; ++pref) {
            std::string buf;
            brute(buf, len, pref, n, rem, pw);
        }

        if (!fuckingShit.empty()) {
            for (auto& s : fuckingShit) {
                std::sort(s.begin(), s.end());
            }
            std::cout << *std::min_element(fuckingShit.begin(), fuckingShit.end()) << '\n';
            break;
        }
    }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

//   int t; cin >> t;
    int t = 1;
  while (t--) solveTest();
  return 0;
}

