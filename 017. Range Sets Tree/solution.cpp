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

struct Node {  // неявное дерево отрезков с получением суммы на отрезке и прибавлением на отрезке
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

struct Cmp{  // функция сравнения пар:
    bool operator()(pair <int, int> a, pair <int, int> b) const {  
        if (a.first < b.first)
            return 1;  // пара a < b
        else if (a.first == b.first)
            return a.second < b.second;
        return 0;
    }
};

void print(set <pair <int, int>, Cmp> &x) {
    cout << "!!!!!" << endl;
    for (auto p: x) {
        cout << "(" << p.first << " " << p.second << "), ";
    }
   cout << "\n!!!!!" << endl;
}

// удаление [l,r) из структуры для элемента x
void x_delete(int l, int r, set <pair <int, int>, Cmp> &x, Node* root) {

    // сначала перебираем все curr - такие интервалы (а точнее итератор на интервал) в структуре, начало которых >= l и < r
    // сам интервал выглядит как [curr->first, curr->second)
    auto curr = x.lower_bound({l, 0});
    while (curr != x.end() && curr->first < r) {  // l <= ... curr->first ... < r  - такие curr перебираем
        if (curr->second <= r) {  // если и конец интервала curr <= r
            add(root, curr->first, curr->second, -1);  // значит весь интервал curr целиком накрывается удаляемым [l, r) -> удалем curr целиком
        } else {  // если же конец curr правее r                                                        (точнее удалим curr далее, а тут -1 делаем)
            add(root, curr->first, r, -1);  // удаляем только кусок curr, накрываемый [l, r) - а именно кусок [curr->first, r)
            x.insert({r, curr->second});  // не забываем добавить в структуру оставшийся от curr кусок
        }
        //x.erase(curr);
        //curr = x.lower_bound({l, 0});
        curr = next(curr);  // сначала переходим к следующему curr
        x.erase(prev(curr));  // и уже теперь удаляем предыдущий curr - чтобы не потерять итератор после удаления
    }
    
    if (x.lower_bound({r, 0}) != x.begin()) {
        curr = prev(x.lower_bound({r, 0}));  // берём curr - самый правый интервал в структуре, начало которого < r (если такой есть - это if проверяет)
        //assert(curr->first < l);  // более того, начало этого интервала будет обязательно левее l, так как все интервалы правее удалили до этого

        if (l < curr->second) {  // если правый конец curr > l, значит curr идёт в нахдёст с [l, r)
                                 // то есть либо так:   curr->first ... l ... curr->second ... r
            add(root, l, min(curr->second, r), -1);  // удаляем кусоечк между l и r
            x.insert({curr->first, l});  // оставшийся кусок слева добавляем в структуру
            if (r < curr->second)  // либо может быть так:   curr->first ... l ... r ... curr->second
                x.insert({r, curr->second});  // тогда оставшийся справа кусок тоже добавляем
            x.erase(curr);  // но весь исходный curr удаляем
        }
    }
}

// добавление [l, r) в структуру для элемента x
void x_add(int l, int r, set <pair <int, int>, Cmp> &x, Node* root) {
    x_delete(l, r, x, root);  // сначала удаляем [l, r) -> в итоге [l,r) не пересекатеся с другими ->
    x.insert({l, r});  // -> добавлеям целиком интервал в структуру
    add(root, l, r, 1);  // и делаем + 1 целиком на интервале
}

int main() {
    /*
    решаем, как говорилось на разборе:
    для каждого x храним некоторую структуру, которая содержит неперескающиеся интревалы [l, r), означающий, что этот x
    находится в множествах с номерами >= l и < r
    в качестве этой структуры используем set() пар с функцией сравнения Cmp

    также есть дерево отрезков, где отрезок [l, r) хранит сумму размеров множеств от l до r

    структура для какого-то конкретного x работает так:
    в ней есть какие-то интервалы: [1, 5), [28, 35), [70, 86), [90, 91), [91, 98] - эти интервалы не пересекаются и вместе содержат те
    номера множеств, в которые входит данный элемент x (то, что интервалы касаются в 91 - не портит структуру);
    далее может прийти запрос - 4 80 x - то есть x удаляется из множеств с номерами [4, 81) -> для этого мы просто этот
    интервал удаляем из структуры, причём так, чтобы они снова не пересекались (просто если удаляемый интервал накрывает какой-то
    имеющийся интервал, то он удаляеется; если он накрывает только кусок интервала - то удаляется этот кусок...), причём для тех интервалов,
    которые удаляются мы делаем -1 из дерева отрезков (показывая, что для множеств с такими номерами один из элементов исчез), а для остальных
    интервалов - ничего не делаем (так как если элемента не было у множестве, то при удалении его ничего не менется в множестве) -> то есть
    поле удаления структура станет такой:
    [1, 4), [81, 86), [90, 91), [91, 98)
    - из неё удалили куски [4, 5), [28, 35), [70, 81) - именно на этих интервалах делаем -1 из дерева отрезков.

    при добавлении интервала в структуру - например, + 50 58 x - мы добавляем в множество интервал [50, 59) - если он не пересекается
    ни с одним из интервалов в структуре, то мы его просто добавляем:
    [1, 4), [81, 86), [50, 59), [90, 91), [91, 98) - получилорсь так
    и делаем + 1 в дереве отрезков на [50, 59)
    если же добавляемый интеврал пересекался бы с интервалом в структуре, то мы должны были добавить в струткуру только те куски добавляемого
    интервала, которые не пересекаются с имеющимися... и на этих кусочках сделать + 1... но чтобы не возиться с э тим, можно
    сделать проще: сначала удалить добавляемый интервал из струткуры (при этом все пересекающиеся куски удалятся и сделается -1...), а потом
    уже добавить этот интервал целиком (и целиком сделать + 1), так как после удаления он точно ни с каким не пересекается.

    в коде добавление в структуру делает x_add, а удлаение x_delete.
    */
    vector <set <pair <int, int>, Cmp>> sets_x(int(1e5)+1, set <pair <int, int>, Cmp>());

    int n, q;
    cin >> n >> q;

    Node* root = new Node(0, int(1e9) + 100);  // неявное дерево отрезков

    for (int i = 0; i < q; i ++) {
        //cout << i << " ! ";
        char t;
        int l, r, x, k;
        cin >> t;

        if (t == '+') {
            cin >> l >> r >> x;  // элемент x добавляется в множества с номерами из [l, r) ->
            x_add(l, r+1, sets_x[x], root);  // -> просто добавляея этот отрезок в структуру для элемента x (и в дереве отрезков +1 делаем)
            //print(sets_x[x]);
        }

        if (t == '-') {
            cin >> l >> r >> x;  // элемент x удаляется из множеств с номерами из [l, r) ->
            x_delete(l, r+1, sets_x[x], root);  // -> просто удаляем этот отрезок из структуры для элемента x (и в дереве отрезков -1 делаем)
            //print(sets_x[x]);
        }

        if (t == '?') {
            cin >> k;
            cout << sum_get(root, k, k+1) << endl;  // выводим размер k множества
        }
    }

    return 0;
}
