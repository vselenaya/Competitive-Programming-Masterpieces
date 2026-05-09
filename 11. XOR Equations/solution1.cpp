// Created by Nikolay Budin

#ifdef DEBUG
#  define _GLIBCXX_DEBUG
#endif
#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#define ff first
#define ss second
#define szof(x) ((int)x.size())
#ifndef LOCAL
#  define cerr __get_ce
#endif

using namespace std;
typedef long long ll;
typedef long double ld;
typedef pair<int, int> pii;
typedef unsigned long long ull;

using namespace __gnu_pbds;
template <typename T> using ordered_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;
template <typename K, typename V> using ordered_map = tree<K, V, less<K>, rb_tree_tag, tree_order_statistics_node_update>;

int const INF = (int)1e9 + 1e3;
ll const INFL = (ll)1e18 + 1e6;
mt19937 tw(9450189);
uniform_int_distribution<ll> ll_distr;
ll rnd(ll a, ll b) { return ll_distr(tw) % (b - a + 1) + a; }

void xor_eq_vec(vector<char>& a, vector<char> const& b) {
	for (int i = 0; i < szof(a); ++i) {
		a[i] ^= b[i];
	}
}

string gen_num(int l) {
	if (l == 1) {
		return string(1, rnd('0', '9'));
	}
	string ret(1, rnd('1', '9'));
	for (int i = 1; i < l; ++i) {
		ret += rnd('0', '9');
	}
	return ret;
}

const int MAXX = 50 * 7;

void solve() {
	map<int, pair<vector<string>, vector<tuple<vector<char>, pii, bitset<MAXX>>>>> mem;
	for (int l = 10; l <= 50; ++l) {
		// vector<vector<char>> strs;
		auto& strs = mem[l].ff;
		// vector<pair<vector<char>, pii>> matr;
		auto& matr = mem[l].ss;
		auto add_str = [&](vector<char> st) {
			if (szof(st) != l) {
				return;
			}
			assert(szof(st) == l);
			string tmp(st.begin(), st.end());
			// cerr << "add " << tmp << endl;
			// assert(!(st[1] & (1 << 6)) && !(st[l - 2] & (1 << 6)));
			// auto qwe = st;
			bitset<MAXX> flipped;
			for (auto& p : matr) {
				if (st[get<1>(p).ff] & (1 << get<1>(p).ss)) {
					xor_eq_vec(st, get<0>(p));
					flipped ^= get<2>(p);
				}
			}

			bool ok = false;
			pii any = {-1, -1};
			for (int i = 0; i < l; ++i) {
				if (st[i]) {
					ok = true;
					any = {i, __builtin_ctz(st[i])};
					break;
				}
			}

			if (ok) {
				flipped[szof(matr)] = flipped[szof(matr)] ^ 1;
				assert(st[any.ff] & (1 << any.ss));
				strs.push_back(tmp);
				matr.push_back({st, any, flipped});
			}
		};

		if (10 <= l - 2) {
			for (auto qwe : mem[l - 2].ff) {
				string s = qwe + "*" + gen_num(1);
				add_str(vector<char>(s.begin(), s.end()));
				s = gen_num(1) + "*" + qwe;
				add_str(vector<char>(s.begin(), s.end()));

				s = qwe + "-0";
				add_str(vector<char>(s.begin(), s.end()));
				s = "0-" + qwe;
				add_str(vector<char>(s.begin(), s.end()));

				s = qwe + "+0";
				add_str(vector<char>(s.begin(), s.end()));
				s = "0+" + qwe;
				add_str(vector<char>(s.begin(), s.end()));

				s = qwe;
				int pos = find(qwe.begin(), qwe.end(), '=') - qwe.begin();
				s.insert(s.begin() + pos + 1, '{');
				s.push_back('}');
				add_str(vector<char>(s.begin(), s.end()));

				s = qwe;
				pos = find(qwe.begin(), qwe.end(), '=') - qwe.begin();
				s.insert(s.begin() + pos, '}');
				s.insert(s.begin(), '{');
				add_str(vector<char>(s.begin(), s.end()));
			}
		}
		if (10 <= l - 3) {
			for (auto qwe : mem[l - 3].ff) {
				string s = qwe + "*" + gen_num(2);
				add_str(vector<char>(s.begin(), s.end()));
				s = gen_num(2) + "*" + qwe;
				add_str(vector<char>(s.begin(), s.end()));
			}
		}

		vector<pair<string, string>> brackets = {{"(", ")"}, {"[", "]"}, {"{", "}"}};


		for (int i = 3; i <= l - 4; ++i) {
			for (int j = 0; j < 10; ++j) {
				string a = gen_num(i - 2);
				string b = gen_num(l - i - 1 - 2);
				if (rnd(0, 1)) {
					a = "0*" + a;
				} else {
					a = a + "*0";
				}
				if (rnd(0, 1)) {
					b = "0*" + b;
				} else {
					b = b + "*0";
				}

				string cur = a + "=" + b;
				assert(szof(cur) == l);
				add_str(vector<char>(cur.begin(), cur.end()));
			}
		}


		function<string(int)> gen_inb = [&](int left) {
			if (left == 0) {
				string ret = "0";
				return ret;
			}
			int q = rnd(0, 2);
			string ret = brackets[q].ff + gen_inb(left - 1) + brackets[q].ss;
			return ret;
		};

		vector<string> signs = {"+", "-", "*"};

		auto gen_exp = [&](int l) {
			int qq = 0;
			while ((qq + 1) * 2 + 1 <= l) {
				++qq;
			}
			qq = rnd(0, qq);
			vector<int> lens(qq + 1, 1);
			for (int i = qq + 1; i < l - qq; ++i) {
				lens[rnd(0, qq)]++;
			}

			string ret = "";
			for (int i = 0; i <= qq; ++i) {
				ret += gen_num(lens[i]);
				if (i < qq) {
					ret += signs[rnd(0, 2)];
				}
			}
			return ret;
		};

		if (l % 2 == 1) {
			for (int i = 0; i < 100; ++i) {
				string tmp = gen_exp(l / 2);
				string s = tmp + "=" + tmp;
				add_str(vector<char>(s.begin(), s.end()));
			}
		}

		for (int i = 0; i < 10; ++i) {
			if (l % 2 == 1) {
				int tot = (l - 3) / 2;
				for (int l1 = 0; l1 <= tot; ++l1) {
					int l2 = tot - l1;
					string cur = gen_inb(l1) + "=" + gen_inb(l2);
					// cerr << cur << endl;
					add_str(vector<char>(cur.begin(), cur.end()));
				}

				tot = (l - 5) / 2;
				for (int l1 = 0; l1 <= tot; ++l1) {
					int l2 = tot - l1;
					string cur = gen_inb(l1) + "=" + gen_inb(l2);
					if (rnd(0, 1)) {
						cur = "0+" + cur;
					} else {
						cur = cur + "+0";
					}
					// cerr << cur << endl;
					add_str(vector<char>(cur.begin(), cur.end()));
				}
			} else {
				int tot = (l - 6) / 2;
				for (int l1 = 0; l1 <= tot; ++l1) {
					int l2 = tot - l1;
					string cur = gen_inb(l1) + "=" + gen_inb(l2);
					if (rnd(0, 1)) {
						cur = gen_num(2) + "*" + cur;
					} else {
						cur = cur + "*" + gen_num(2);
					}
					// cerr << cur << endl;
					add_str(vector<char>(cur.begin(), cur.end()));
				}
			}
		}

		for (int i = 0; i < 10; ++i) {
			string a = gen_num(l - 4);
			if (rnd(0, 1)) {
				a = a + "*0";
			} else {
				a = "0*" + a;
			}

			if (rnd(0, 1)) {
				a = a + "=0";
			} else {
				a = "0=" + a;
			}

			add_str(vector<char>(a.begin(), a.end()));

			for (auto [ob, cb] : brackets) {
				a = ob + gen_num(l - 6) + cb;
				if (rnd(0, 1)) {
					a = a + "*0";
				} else {
					a = "0*" + a;
				}

				if (rnd(0, 1)) {
					a = a + "=0";
				} else {
					a = "0=" + a;
				}

				add_str(vector<char>(a.begin(), a.end()));

				for (int l1 = 1; l1 < l - 7; ++l1) {
					for (auto sign : signs) {
						a = ob + gen_num(l1) + sign + gen_num(l - 7 - l1) + cb;
						if (rnd(0, 1)) {
							a = a + "*0";
						} else {
							a = "0*" + a;
						}

						if (rnd(0, 1)) {
							a = a + "=0";
						} else {
							a = "0=" + a;
						}

						add_str(vector<char>(a.begin(), a.end()));
					}
				}

				for (auto [ob2, cb2] : brackets) {
					a = ob + gen_num(l - 8) + cb;
					if (rnd(0, 1)) {
						a = a + "*0";
					} else {
						a = "0*" + a;
					}

					if (rnd(0, 1)) {
						a = a + "=" + ob2 + "0" + cb2;
					} else {
						a = ob2 + "0" + cb2 + "=" + a;
					}

					add_str(vector<char>(a.begin(), a.end()));

					a = ob + gen_num(l - 8) + cb;
					if (rnd(0, 1)) {
						a = a + "*" + ob2 + "0" + cb2;
					} else {
						a = ob2 + "0" + cb2 + "*" + a;
					}

					if (rnd(0, 1)) {
						a = a + "=0";
					} else {
						a = "0=" + a;
					}

					add_str(vector<char>(a.begin(), a.end()));
				}
			}
		}

		for (int i = 0; i < l; ++i) {
			for (auto [ob, cb] : brackets) {
				for (int tr = 0; tr < 1; ++tr) {
					string cur;
					if (i >= 2 && l - i - 6 >= 1) {
						cur = gen_num(i - 1) + "*" + ob + "0" + cb + "=0*" + gen_num(l - i - 6);
					}
					if (i == 0 && l - i - 6 >= 1) {
						cur = ob + "0" + cb + "=0*" + gen_num(l - i - 6);
					}
					if (i >= 2 && l - i == 5) {
						cur = gen_num(i - 1) + "*" + ob + "0" + cb + "=0";
					}
					if (szof(cur)) {
						add_str(vector<char>(cur.begin(), cur.end()));
					}

					cur.clear();
					if (l - i - 1 >= 2 && i - 5 >= 1) {
						cur = gen_num(i - 5) + "*0=" + ob + "0" + cb + "*" + gen_num(l - i - 2);
					}
					if (l - i - 1 >= 2 && i - 4 == 0) {
						cur = "0=" + ob + "0" + cb + "*" + gen_num(l - i - 2);
					}
					if (i == l - 1) {
						cur = gen_num(i - 5) + "*0=" + ob + "0" + cb;
					}
					if (szof(cur)) {
						add_str(vector<char>(cur.begin(), cur.end()));
					}
				}
			}
		}

		for (int i = 0; i < 100; ++i) {
			string s = "0=0*{" + gen_exp(l - 6) + "}";
			// cerr << s << endl;
			add_str(vector<char>(s.begin(), s.end()));
			s = "0=0*{{" + gen_exp(l - 8) + "}}";
		}



		// cerr << szof(matr) << " out of " << l * 7 << " (" << l * 7 - szof(matr) << ")\n";
		
		/*
		vector<vector<bool>> used(l, vector<bool>(7));
		for (auto& p : matr) {
			used[get<1>(p).ff][get<1>(p).ss] = true;
		}

		cerr << "need:" << endl;
		for (int i = 0; i < l; ++i) {
			for (int j = 0; j < 7; ++j) {
				if (!used[i][j]) {
					cerr << i << " " << j << endl;
				}
			}
		}
		*/
	}

	for (int l = 11; l <= 11; ++l) {
		auto& strs = mem[l].ff;
		// vector<pair<vector<char>, pii>> matr;
		auto& matr = mem[l].ss;
		auto add_str = [&](vector<char> st) {
			if (szof(st) != l) {
				return;
			}
			assert(szof(st) == l);
			string tmp(st.begin(), st.end());
			// cerr << "add " << tmp << endl;
			// assert(!(st[1] & (1 << 6)) && !(st[l - 2] & (1 << 6)));
			// auto qwe = st;
			bitset<MAXX> flipped;
			for (auto& p : matr) {
				if (st[get<1>(p).ff] & (1 << get<1>(p).ss)) {
					xor_eq_vec(st, get<0>(p));
					flipped ^= get<2>(p);
				}
			}

			bool ok = false;
			pii any = {-1, -1};
			for (int i = 0; i < l; ++i) {
				if (st[i]) {
					ok = true;
					any = {i, __builtin_ctz(st[i])};
					break;
				}
			}

			if (ok) {
				flipped[szof(matr)] = flipped[szof(matr)] ^ 1;
				assert(st[any.ff] & (1 << any.ss));
				strs.push_back(tmp);
				matr.push_back({st, any, flipped});
			}
		};

		vector<pair<string, string>> brackets = {{"(", ")"}, {"[", "]"}, {"{", "}"}};

		vector<string> signs = {"+", "-", "*"};

		auto gen_exp = [&](int l) {
			int qq = 0;
			while ((qq + 1) * 2 + 1 <= l) {
				++qq;
			}
			qq = rnd(0, qq);
			vector<int> lens(qq + 1, 1);
			for (int i = qq + 1; i < l - qq; ++i) {
				lens[rnd(0, qq)]++;
			}

			string ret = "";
			for (int i = 0; i <= qq; ++i) {
				ret += gen_num(lens[i]);
				if (i < qq) {
					ret += signs[rnd(0, 2)];
				}
			}
			return ret;
		};

		for (auto [ob, cb] : brackets) {
			string tmp = gen_exp((11 - 3) / 2);
			string s = tmp + "=" + ob + tmp + cb;
			add_str(vector<char>(s.begin(), s.end()));
			// cerr << s << endl;
		}

		// cerr << szof(matr) << endl;
	}

	// for (int l = 10; l <= 50; ++l) {
	// 	for (string s : mem[l].ff) {
	// 		cout << s << endl;
	// 	}
	// }

	int q;
	cin >> q;
	for (int tn = 0; tn < q; ++tn) {
		string s;
		cin >> s;
		auto& strs = mem[szof(s)].ff;
		auto& matr = mem[szof(s)].ss;
		vector<char> cur(s.begin(), s.end());
		bitset<MAXX> flipped;
		for (int i = 0; i < szof(matr); ++i) {
			auto& p = matr[i];
			if (cur[get<1>(p).ff] & (1 << get<1>(p).ss)) {
				xor_eq_vec(cur, get<0>(p));
				flipped ^= get<2>(p);
			}
		}

		bool ok = true;
		for (auto c : cur) {
			if (c) {
				ok = false;
				break;
			}
		}
		vector<string> outp;
		cur = vector<char>(s.begin(), s.end());
		for (int i = 0; i < szof(matr); ++i) {
			if (flipped[i]) {
				outp.push_back(strs[i]);
				vector<char> qwe(strs[i].begin(), strs[i].end());
				xor_eq_vec(cur, qwe);
			}
		}

		if (!ok) {
			cout << "NO\n";
		} else {
			assert(cur == vector<char>(szof(s)));
			cout << "YES\n";
			cout << szof(outp) << "\n";
			for (auto& w : outp) {
				cout << w << "\n";
			}
		}
	}
}


int main() {
#ifdef LOCAL
	auto start_time = clock();
	cerr << setprecision(3) << fixed;
#endif
	cout << setprecision(15) << fixed;
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int test_count = 1;
	// cin >> test_count;
	for (int test = 1; test <= test_count; ++test) {
		solve();
	}

#ifdef LOCAL
	auto end_time = clock();
	cerr << "Execution time: " << (end_time - start_time) * (int)1e3 / CLOCKS_PER_SEC << " ms\n";
#endif
}
