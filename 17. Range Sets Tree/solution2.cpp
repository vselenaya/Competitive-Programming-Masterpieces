
#include <iostream>
#include <cmath>
#include <vector> 
#include <string>
#include <algorithm>
#include <utility>

#include <queue>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>

using namespace std;

struct Node {
    long long s;
    long long delta;
    long long l, r;    
    Node *left, *right;
    
    Node () {
        delta = 0;
        left = 0;
        right = 0;
    }
    
    Node (long long l, long long r) {
        s = 0;
        delta = 0;
        left = 0;
        right = 0;
        this->l = l;
        this->r = r;
    }       
    
    void make_next() {
        if (r <= l + 1)
            return;
        
        long long m = (0ll + l + r) / 2;
        
        if (left == 0)
            left = new Node(l, m);
        if (right == 0)
            right = new Node(m, r);
    }       
    
    void push() {
        if (left) {
            left->s += delta * (left->r - left->l) * 1ll;
            left->delta += delta;
        }
        
        if (right) {
            right->s += delta * (right->r - right->l) * 1ll;
            right->delta += delta;
        }
        
        delta = 0;
    }          
};

void add(Node* curr, long long l, long long r, long long value) {
    if (l >= r || curr->r <= l || r <= curr->l)
        return;
    
    if (curr->l == curr->r - 1) {
        curr->s += value;
        return;
    }
    
    if (l <= curr->l && curr->r <= r) {
        curr->s += value * (min(r, curr->r) - max(l, curr->l));
        curr->delta += value;
        return;
    }
    
    curr->make_next();
    curr->push();
    
    add(curr->left, l, r, value);
    add(curr->right, l, r, value);
    
    curr->s = curr->left->s + curr->right->s;
    
    return;
}

long long sum_get(Node* curr, long long l, long long r) {
    if (curr == 0)
        return 0;
    
    if (l >= r || curr->r <= l || r <= curr->l)
        return 0;
    
    if (l <= curr->l && curr->r <= r) {
        return curr->s;
    }
    
    if (curr->delta != 0) {
        curr->make_next();
        curr->push();
    }
    
    return sum_get(curr->left, l, r) + sum_get(curr->right, l, r);
}

struct Cmp{
    bool operator()(pair <int, int> a, pair <int, int> b) const {
        if (a.first < b.first)
            return 1;
        else if (a.first == b.first)
            return a.second > b.second;
        return 0;
    }
};

void print(set <pair <int, int>, Cmp> &x) {
    //cout << "!!!!!" << endl;
    for (auto p: x) {
        cout << "(" << p.first << " " << p.second << "), ";
    }
   // cout << "\n!!!!!" << endl;
}

void sprint(Node* root, set <pair <int, int>, Cmp> &x) {
    cout << "!!!!!" << endl;

    cout << "true: ";
    for (int i = 1; i <= 100; i ++)
        cout << sum_get(root, i, i+1);
    cout << endl;
    
    cout << "se29: ";
    int st = 1;
    for (auto p: x) {
        if (p.second == -1) {
            for (int i = st; i < p.first; i ++)
                cout << '0';
            st = p.first;
        } else {
           for (int i = st; i < p.first; i ++)
                cout << '1';
            st = p.first; 
        }
    }
    for (int i = st; i <= 100; i ++)
        cout << '0';
    cout << "   ";
    print(x);
    cout << "\n!!!!!" << endl;
}

void x_add_1(int l, int r, set <pair <int, int>, Cmp> &x, Node* root) {
    auto _R = x.lower_bound({r, 1});
    auto next_L_ = x.upper_bound({l, -1});
    if (_R != x.end() && prev(next_L_) != x.end() && next_L_ == _R && _R->second == 1)
        return;

    if (l >= r)
        return;

    add(root, l, r, 1);
    
    //cout << "1111";
    if (x.upper_bound({l, -1})->first != r && x.upper_bound({l, -1})->second == 1 && prev(x.upper_bound({l, -1}))->first < l) {
        x.insert({l, 1});
    }

    if (x.upper_bound({r, 1}) != x.end() && x.upper_bound({r, 1})->second == 1)
        x.insert({r, -1});

    x.insert({l, -1});
    x.insert({r, 1});
    
    auto L = x.find({l, -1});
    auto R = x.find({r, 1});
    
    if (next(L) == R)
        return;   
    
    auto curr = L;
    while (curr != R) {
        if (curr->second == -1 && next(curr)->second == 1)
            add(root, curr->first, next(curr)->first, -1);
        curr = next(curr);
        if (prev(curr) != L)
            x.erase(prev(curr));
    }
}

void x_delete_1(int l, int r, set <pair <int, int>, Cmp> &x, Node* root) {
    if ((x.size() == 0) || (r <= x.begin()->first) || (l >= prev(x.end())->first)) {
        return;
    }

    auto _R = x.lower_bound({r, 1});
    auto next_L_ = x.upper_bound({l, -1});

    if (x.upper_bound({l, -1}) != x.end() && x.upper_bound({l, -1})->second == 1 && prev(x.upper_bound({l, -1}))->first < l)
        x.insert({l, 1});

    if (x.upper_bound({r, 1}) != x.end() && x.upper_bound({r, 1})->second == 1)
        x.insert({r, -1});
    
    if (_R != x.end() && prev(next_L_) != x.end() && next_L_ == _R && _R->second == 1) {
        add(root, l, r, -1);
        x.erase({l, -1});
        x.erase({r, 1});
        return;
    }

    x.insert({l, -1});
    x.insert({r, 1});
    
    auto L = x.find({l, -1});
    auto R = x.find({r, 1});

    if (next(L) == R) {
        x.erase(L);
        x.erase(R);
        return;
    }
    auto curr = L;
    while (curr != R) {
        if (curr->second == -1 && next(curr)->second == 1)
            add(root, curr->first, next(curr)->first, -1);
        curr = next(curr);
        x.erase(prev(curr));
    }
    x.erase(R);
} 

int main() {

    vector <set <pair <int, int>, Cmp>> sets_x(int(1e5)+1, set <pair <int, int>, Cmp>());

    int n, q;
    cin >> n >> q;

    Node* root = new Node(0, int(1e9) + 100);

    for (int i = 0; i < q; i ++) {
        //cout << i << " ! ";
        char t;
        int l, r, x, k;
        cin >> t;

        if (t == '+') {
            cin >> l >> r >> x;
            x_add_1(l, r+1, sets_x[x], root);
            //sprint(root, sets_x[x]);
        }

        if (t == '-') {
            cin >> l >> r >> x;
            x_delete_1(l, r+1, sets_x[x], root);
            //sprint(root, sets_x[x]);
        }

        if (t == '?') {
            cin >> k;
            cout << sum_get(root, k, k+1) << endl;
        }

        if (t == 'p') {
            cin >> k;
            print(sets_x[k]);
        }

        if (t == 's') {
            cin >> k;
            sprint(root, sets_x[k]);
        }

    }

    return 0;
}
