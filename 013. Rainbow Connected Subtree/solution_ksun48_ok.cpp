/*
File name: 230975722_ksun48_ok.cpp
Tag: ACCEPTED
Author: codeforces
Change time: Fri Nov 03 03:41:07 MSK 2023
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

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define trav(a, x) for(auto& a : x)
#define all(x) x.begin(), x.end()
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

struct TwoSat {
	int N;
	vector<vi> gr;
	vi values; // 0 = false, 1 = true

	TwoSat(int n = 0) : N(n), gr(2*n) {}

	int addVar() { // (optional)
		gr.emplace_back();
		gr.emplace_back();
		return N++;
	}

	void either(int f, int j) {
		f = max(2*f, -1-2*f);
		j = max(2*j, -1-2*j);
		gr[f].push_back(j^1);
		gr[j].push_back(f^1);
	}
	void setValue(int x) { either(x, x); }

	void atMostOne(const vi& li) { // (optional)
		if (sz(li) <= 1) return;
		int cur = ~li[0];
		rep(i,2,sz(li)) {
			int next = addVar();
			either(cur, ~li[i]);
			either(cur, next);
			either(~li[i], next);
			cur = ~next;
		}
		either(cur, ~li[1]);
	}

	vi val, comp, z; int time = 0;
	int dfs(int i) {
		int low = val[i] = ++time, x; z.push_back(i);
		for(int e : gr[i]) if (!comp[e])
			low = min(low, val[e] ?: dfs(e));
		if (low == val[i]) do {
			x = z.back(); z.pop_back();
			comp[x] = low;
			if (values[x>>1] == -1)
				values[x>>1] = x&1;
		} while (x != i);
		return val[i] = low;
	}

	bool solve() {
		values.assign(N, -1);
		val.assign(2*N, 0); comp = val;
		rep(i,0,2*N) if (!comp[i]) dfs(i);
		rep(i,0,N) if (comp[2*i] == comp[2*i+1]) return 0;
		return 1;
	}
};

void solve(){
	int N;
	cin >> N;
	N *= 2;
	vector<int> type(N);
	for(int i = 0; i < N; i++){
		cin >> type[i];
		type[i]--;
	}
	vector<int> label(N);
	vector<vector<int> > which(N);
	for(int i = 0; i < N; i++){
		if(which[type[i]].empty()){
			label[i] = type[i];
		} else {
			label[i] = ~type[i];
		}
		which[type[i]].push_back(i);
	}
	vector<int> roots;
	for(int i = 0; i < N; i++) if(type[i] == 0) roots.push_back(i);

	vector<vector<int> > tree(N);
	for(int i = 0; i < N-1; i++){
		int u, v;
		cin >> u >> v;
		u--; v--;
		tree[u].push_back(v);
		tree[v].push_back(u);
	}
	for(int root : roots){
		vector<int> par(N);
		y_combinator(
			[&](auto self, int v, int p) -> void {
				par[v] = p;
				for(int w : tree[v]){
					if(w == p) continue;
					self(w, v);
				}
			}
		)(root, -1);
		TwoSat twosat(N/2);
		for(int i = 0; i < N; i++){
			if(i == root) continue;
			twosat.either(~label[i], label[par[i]]);
		}
		twosat.setValue(label[root]);
		bool ans = twosat.solve();
		if(ans){
			for(int i = 0; i < N/2; i++){
				cout << (1 + which[i][!twosat.values[i]]) << " \n"[i+1 == N/2];
			}
			return;
		}
	}
	cout << -1 << '\n';
}

int main(){
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int T;
	cin >> T;
	while(T--) solve();
}
