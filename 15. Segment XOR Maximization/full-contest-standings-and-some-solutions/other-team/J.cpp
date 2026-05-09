#include <bits/stdc++.h>

using namespace std;

using ll = long long;

const int N = 10;

ll f[N + 1];

void precalc() {
    f[0] = 1;
    for (int i = 1; i < N + 1; ++i) {
        f[i] = f[i - 1] * i;
    }
}

int getNum(const std::string& s) {
    std::string sortedS = s;
    std::sort(sortedS.begin(), sortedS.end());
    std::set<char> used;

    int ans = 0;
    for (int i = 0; i < (int)s.size(); ++i) {
        char cur = s[i];
        for (char c : sortedS) {
            if (used.find(c) != used.end()) {
                continue;
            }
            if (c < cur) {
                ans += f[(int)s.size() - i - 1];
            } else {
                break;
            }
        }
        used.insert(cur);
    }

    return ans;
}

std::string byNum(int x, const std::string& sortedS) {
    std::string ans;
    std::set<char> used;
    for (int i = 0; i < (int)sortedS.size(); ++i) {
        for (auto cur : sortedS) {
            if (used.find(cur) != used.end()) {
                continue;
            }
            if (x - f[(int)sortedS.size() - i - 1] >= 0) {
                x -= f[(int)sortedS.size() - i - 1];
                continue;
            }
            ans.push_back(cur);
            used.insert(cur);
            break;
        }
    }
    return ans;
}

void encode() {
    std::string s;
    std::cin >> s;
    
    int num = getNum(s);
    int globalPermId = num / (f[7] * f[5]);
    int localPermId = num % (f[7] * f[5]);
    assert(globalPermId < 6);

    std::string a = "A";
    std::string sortedB = "ABCDEFG";
    std::string sortedC = "ABCDE";
    int firstId = localPermId / f[5];
    int secondId = localPermId % f[5];
    assert(firstId < f[7]);

    std::string b = byNum(firstId, sortedB);
    std::string c = byNum(secondId, sortedC);

    std::vector<std::string> ans = {a, b, c};
    int ind[3] = {0, 1, 2};
    while (globalPermId--) {
        std::next_permutation(ind, ind + 3);
    }

    for (int i = 0; i < 3; ++i) {
        int curInd = ind[i];
        std::cout << ans[curInd] << ' ';
    }
    std::cout << std::endl;
}

void decode() {
    std::string a, b, c;
    std::cin >> a >> b >> c;

    std::vector<std::string> input = {a, b, c};
    int globalPermId = 0;
    std::vector<int> perm = {0, 0, 0};
    if (a.size() == 1 && b.size() == 7 && c.size() == 5) {
        globalPermId = 0;
        perm = {0, 1, 2};
    } else if (a.size() == 1 && b.size() == 5 && c.size() == 7) {
        globalPermId = 1;
        perm = {0, 2, 1};
    } else if (a.size() == 7 && b.size() == 1 && c.size() == 5) {
        globalPermId = 2;
        perm = {1, 0, 2};
    } else if (a.size() == 7 && b.size() == 5 && c.size() == 1) {
        globalPermId = 3;
        perm = {1, 2, 0};
    } else if (a.size() == 5 && b.size() == 1 && c.size() == 7) {
        globalPermId = 4;
        perm = {2, 0, 1};
    } else if (a.size() == 5 && b.size() == 7 && c.size() == 1) {
        globalPermId = 5;
        perm = {2, 1, 0};
    } else {
        assert(false);
    }

    // std::cerr << globalPermId << std::endl;

    std::vector<int> revPerm(3);
    for (int i = 0; i < 3; ++i) {
        revPerm[perm[i]] = i;
    }

    int firstId = getNum(input[revPerm[1]]);
    int secondId = getNum(input[revPerm[2]]);

    int num = globalPermId * f[5] * f[7];
    num += firstId * f[5];
    num += secondId;
    // int num = ((secondId * f[5] + firstId) * f[7]) + globalPermId;
    // std::cerr << firstId << ' ' << secondId << ' ' << num << std::endl;
    std::string ans = byNum(num, "ABCDEFGHIJ");
    std::cout << ans << std::endl;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  precalc();

  std::string mode;
  int t;
  std::cin >> mode >> t;

  if (mode == "coach") {
    while (t--) {
        encode();
    }
  } else if (mode == "team") {
    while (t--) {
        decode();
    }
  } else {
    assert(false);
  }
  
  return 0;
}

