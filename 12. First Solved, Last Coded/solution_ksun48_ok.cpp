/*
File name: 230961546_ksun48_ok.cpp
Tag: ACCEPTED
Author: codeforces
Change time: Thu Nov 02 16:48:18 MSK 2023
*/

#include <bits/stdc++.h>
using namespace std;

namespace std {

template<class Fun>
class y_combinator_result {
	Fun fun_;
public:
	template<class T>
	explicit y_combinator_result(T &&fun): fun_(std::forward<T>(fun)) {}

	template<class ...Args>
	decltype(auto) operator()(Args &&...args) {
		return fun_(std::ref(*this), std::forward<Args>(args)...);
	}
};

template<class Fun>
decltype(auto) y_combinator(Fun &&fun) {
	return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun));
}

} // namespace std

int main(){
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N;
	cin >> N;
	vector<int> A(N);
	vector<int> B(N);
	for(int& x : A) cin >> x;
	for(int& x : B) cin >> x;
	vector<vector<vector<int> > > dp(N+1, vector<vector<int> >(N+1, vector<int>(N+1, 0)));
	for(int pA = 0; pA <= N; pA++){
		for(int pB = 0; pB <= pA; pB++){
			dp[0][pA][pB] = 1;
		}
	}
	vector<vector<vector<int> > > nxt(N+1, vector<vector<int> >(N+1, vector<int>(N+1, 0)));

	for(int l = 1; l <= N; l++){
		for(int pA = 0; pA + l <= N; pA++){
			for(int pB = 0; pB <= pA; pB++){
				if(A[pA] == B[pB] && dp[l-1][pA+1][pB+1]){
					dp[l][pA][pB] = 1;
					nxt[l][pA][pB] = -1;
				}
				if(A[pA] == B[pB+l-1] && dp[l-1][pA+1][pB]){
					dp[l][pA][pB] = 1;
					nxt[l][pA][pB] = -2;
				}
				for(int x = 1; x < l; x++){
					if(dp[x][pA][pB] && dp[l-x][pA+x][pB+x]){
						dp[l][pA][pB] = 1;
						nxt[l][pA][pB] = x;
					}
				}
			}
		}
	}

	if(dp[N][0][0]){
		string ans;
		y_combinator(
			[&](auto self, int l, int pA, int pB) -> void {
				assert(dp[l][pA][pB]);
				if(l == 0){
					return;
				}
				int r = nxt[l][pA][pB];
				if(r == -1){
					ans += 'S';
					ans += 'C';
					self(l-1, pA+1, pB+1);
				} else if(r == -2){
					ans += 'S';
					self(l-1, pA+1, pB);
					ans += 'C';
				} else {
					self(r, pA, pB);
					self(l-r, pA+r, pB+r);
				}
			}
		)(N, 0, 0);
		cout << "YES" << '\n';
		cout << ans << '\n';
	} else {
		cout << "NO" << '\n';
	}
}
