#include <iostream>
#include <vector>
#include <map>

/* Это наиболее простое решение, в отличие от других */

using namespace std;

const int MAX_COORD = 1e9 + 10; // Координаты до 10^9

// --- Неявное дерево отрезков без ленивого проталкивания ---
// Запоминаем индексы детей вместо указателей (защита от утечек и перераспределения памяти)
struct Node {
    long long val = 0;
    int l = -1, r = -1;
};
vector<Node> st(1); // 0-й элемент — это всегда корень

void add(int v, int tl, int tr, int l, int r, int val) {
    if (l >= tr || r <= tl) return;
    if (l <= tl && tr <= r) {
        st[v].val += val; // Просто сохраняем дельту прямо в узле
        return;
    }
    int tm = tl + (tr - tl) / 2;
    if (l < tm) {
        if (st[v].l == -1) { st[v].l = st.size(); st.push_back({}); }
        add(st[v].l, tl, tm, l, r, val);
    }
    if (r > tm) {
        if (st[v].r == -1) { st[v].r = st.size(); st.push_back({}); }
        add(st[v].r, tm, tr, l, r, val);
    }
}

long long get_sum(int v, int tl, int tr, int pos) {
    if (v == -1) return 0;
    if (tl == tr - 1) return st[v].val;
    
    int tm = tl + (tr - tl) / 2;
    // Суммируем всё, что накопилось по пути вниз
    if (pos < tm) return st[v].val + get_sum(st[v].l, tl, tm, pos);
    else return st[v].val + get_sum(st[v].r, tm, tr, pos);
}

// --- Менеджер непересекающихся отрезков (идея Chtholly Tree) ---
// Будет свой для каждого x, содержит неперескающиеся интревалы [l, r), означающие, что этот x находится в множествах с номерами >= l и < r
struct IntervalManager {
    map<int, int> m;  // Храним отрезки только по координате начала, конец отрезка же хранится неявно -- отрезок кончается там,
                      // где начинается следующий отрезок...
                      // В данной map по координате начала отрезка -> храним статус отрезка (1 = такой отрезок есть в нашей структуре или 0 = его нет (выколотый отрезок))
    
    IntervalManager() {  // изначально добавляем два отрезка (обоих на самом деле пока нет в структуре, поэтому их статус 0)
        m[0] = 0;   // у первого отрезка начало в 0 (а конец, как обсуждалось, в начале следующего отрезка, то есть в 2e9)
        m[2e9] = 0; // у второго отрезка следующего за ним нет, так что считаем, что это отрезок от 2e9 до бесконечности
    }               // итого, два отрезка (со статусом 0, то есть их как будто нет, они особо ничего и не значат, скорее размечают область): [0, 2e9) и [2e9, inf)

    // Разрезает отрезок, который накрывает точку x, на два
    // Возвращает итератор на отрезок, начинающийся ровно с x
    auto split(int x) {
        auto it = m.lower_bound(x);
        if (it != m.end() && it->first == x) return it; // Уже разрезан тут
        --it; // Берем предыдущий отрезок, который накрывает x (такой всегда есть, хотя бы отрезок m[0])
        int val = it->second;
        return m.insert({x, val}).first;  // вставляем вторую половинку отрезка -- от x и далее (что автоматически укорачивает и предыдущий,
                                                // так как отрезки у нас заканчиваются там, где начинаются следующие)
    }

    // Присвоить значение v (1 - добавить, 0 - удалить) на полуинтервале [l, r)
    void assign(int l, int r, int v) {
        // Обязательно сначала режем правый конец, потом левый!
        auto itR = split(r);
        auto itL = split(l);
        
        // Проходим по всем кусочкам строго внутри [l, r)
        for (auto it = itL; it != itR; ++it) {
            if (it->second != v) { // Если статус меняется
                int curL = it->first;
                int curR = next(it)->first;
                int diff = (v == 1) ? 1 : -1;
                add(0, 0, MAX_COORD, curL, curR, diff); // Обновляем глобальное ДО
            }
        }
        
        // Удаляем всю труху внутри и оставляем один сплошной отрезок
        m.erase(next(itL), itR);
        itL->second = v; 
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, q;
    cin >> n >> q;

    // массив таких менеджеров для каждого возможного x
    // Разреженный вектор, x ограничен размером q, так что 1e5+10 хватит
    vector<IntervalManager> sets_x(100005);

    for (int i = 0; i < q; i++) {
        char t;
        cin >> t;

        if (t == '+') {
            int l, r, x;
            cin >> l >> r >> x;
            sets_x[x].assign(l, r + 1, 1);
        }
        else if (t == '-') {
            int l, r, x;
            cin >> l >> r >> x;
            sets_x[x].assign(l, r + 1, 0);
        }
        else if (t == '?') {
            int k;
            cin >> k;
            cout << get_sum(0, 0, MAX_COORD, k) << endl;  // endl, чтобы сбросить буфер вывода
        }
    }

    return 0;
}
