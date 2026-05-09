#include <bits/stdc++.h>
using namespace std;


vector <int> primes;
const int maxn = 1987;

bool ask(int k){
    cout << "? " << k << endl;
    int answ;
    cin >> answ;
    return answ;
}

int solve(){
    ask(1);
    vector <int> my_primes;
    for (auto p: primes){
        if (p > 1000 - 7 && my_primes.size() > 0)
            break;
        if (!ask(p)){
            my_primes.push_back(p);
            if (p > 1000 - 7)
                return p;
        }
    }
    int ans = 1;
    for (auto q: my_primes){
        int cur = q;
        ans *= q;
        for (int st = 2; st < 20; ++st){
            cur *= q;
            if (cur >= maxn)
                break;
            if (ask(cur)){
                break;
            }
            else{
                ans *= q;
            }
        }
    }
    return ans;
}

int main() {
    vector <bool> pr(maxn, true);
    for (int i = 2; i < maxn; ++i){
//        if (i == 1000 - 7)
//            cout << primes.size() << '\n';
        if (!pr[i]) continue;
        primes.push_back(i);
        int cur = i;
        for (int j = 2 * i; j < maxn; j += i)
            pr[j] = false;
    }
    int t;
    cin >> t;
    while (t--){
        int sol = solve();
        cout << "! " << sol << endl;
    }
}