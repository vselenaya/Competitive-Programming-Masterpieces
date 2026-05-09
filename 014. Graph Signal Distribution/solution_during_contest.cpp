/*
 * =================================================================================================
 * РАЗБОР РЕШЕНИЯ: МЕТОД ТЕРНАРНОГО ПОИСКА
 * =================================================================================================
 * * СТРУКТУРА ЗАДАЧИ:
 * Мы имеем систему линейных уравнений вида x_i + x_j = c_k для каждого ребра. 
 * Это классическая задача на графах, где значения в вершинах зависят друг от друга.
 *
 * ЭТАП 1: ПОИСК НЕЧЕТНОГО ЦИКЛА
 * Если в графе есть цикл нечетной длины, значение переменной x в любой вершине этого цикла 
 * фиксируется ОДНОЗНАЧНО. 
 * Почему? Рассмотрим цикл: v1 + v2 = c1, v2 + v3 = c2, ..., vk + v1 = ck.
 * Сложим уравнения с чередующимися знаками: (v1 + v2) - (v2 + v3) + (v3 + v4) ... + (vk + v1).
 * В нечетном цикле все промежуточные переменные сократятся, и останется: 2 * v1 = c1 - c2 + c3...
 * Следовательно, v1 = (чередующаяся сумма) / 2. Если сумма нечетная — целого решения нет (ответ = No, тк все
 * сигналы по условию целые).
 *
 * ЭТАП 2: РАСПРОСТРАНЕНИЕ ЗНАЧЕНИЙ (DFS)
 * Как только мы узнали x хотя бы в одной вершине (будь то корень или вершина цикла), 
 * значения всех остальных вершин в компоненте связности определяются автоматически:
 * x_neighbor = c_edge - x_current.
 *
 * ЭТАП 3: ОПТИМИЗАЦИЯ ДЛЯ ДВУДОЛЬНОГО ГРАФА
 * Если нечетных циклов нет, граф двудольный. Это значит, что мы можем выбрать произвольное 
 * x_root = X. Тогда все вершины первой доли будут иметь вид (X + const), 
 * а второй доли — (const - X). 
 * Нам нужно минимизировать функцию F(X) = Σ |x_i|. 
 * Математический факт: сумма модулей линейных функций f(X) = Σ |a_i*X + b_i| является ВЫПУКЛОЙ.
 * Визуально это "чаша", у которой есть один глобальный минимум. 
 * Для поиска этого минимума идеально подходит тернарный поиск по значению X.
 *
 * ПОРЯДОК РАБОТЫ КОДА:
 * 1. Ищем нечетный цикл через DFS и раскраску.
 *    (Это делают `dfs` и `add_cycle`, которые фактически проверяют граф на двудольность, крася вершины в 'r' (red) и 'b' (blue).
 *     Параллельно в par dfs запоминает, откуда пришел в вершину. Если мы пытаемся пойти по ребру и видим вершину такого же
 *     цвета — бинго, найден нечётный цикл. Вызывается add_cycle, которая с помощью массива предков par "отматывает" путь назад,
 *     собирая вершины и веса рёбер в массив cycle.)
 * 2. Если нашли: вычисляем фиксированный X, проверяем его на четность и запускаем DFS установки сил.
 *    (вычисления фиксированного X происходят в `main`, а затем распространяются через `dfs_set_power`)
 * 3. Если не нашли: запускаем тернарный поиск (`ternar_search`), который на каждой итерации "проливает" значения
 *    от корня и считает суммарную стоимость (`get_cost`).
 * 4. В конце обязательно проверяем все рёбра: x_u + x_v == c, чтобы отсечь невозможные случаи.
 *
 * =================================================================================================

 P.S. Этот код я писал на контесте, мне не хватило буквально 5 минут, чтобы упихать это решение в TimeLimit (вот зачем так жадно ставить лимит?!)...
 Реализацию ниже я дописал через 5 минут после конца контеста, она проходит все тесты и получает Accepted. Суммарно по всем тестам
 максимальное использование ресурсов: 0.868s (время) и 21.04Mb (память).
 */

#pragma optimization_level 3
#pragma GCC optimize("Ofast,no-stack-protector,unroll-loops,fast-math,O3")
#pragma GCC optimize("Ofast")//Comment optimisations for interactive problems (use endl)
#pragma GCC optimization ("unroll-loops")

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <assert.h>

using namespace std;

void add_cycle (int from, int to, int cost, vector <pair <int, int>> &par, vector <pair <int, int>> &cycle) {
    if (cycle.size() > 0)  // если cylce не пуст, то в нём уже есть цикл -> ничего добавлять не будем
        return;
    
    int cur = from;  // начинаем с вершины from
    int cur_c = cost;
    //cycle.push_back({to, cur_c});
    while (1) {
        if (cur == to) {
            cycle.push_back({to, cost});
            break;
        }
        cycle.push_back({cur, par[cur].second});  // добавляем вершины в вектор
        
        int _cc = cur;
        cur = par[_cc].first;
    }
    reverse(cycle.begin(), cycle.end());  // переворачиваем вектор, тк вершины цила добавляли в обратном порядке
    
    return;
}
    
void dfs (int u, char col, vector <vector <pair <int, int>>> &g, vector <char> &used, vector <pair <int, int>> &par, vector <pair <int, int>> &cycle) {
    used[u] = col;  // красим вершину

    char new_col;
    if (col == 'r')
        new_col = 'b';
    else if (col == 'b')
        new_col = 'r';

    for (auto v: g[u]) {
        if (used[v.first] == col) {  // если нашли ребро в такой же цвет, нашли нечётный цикл
            add_cycle(u, v.first, v.second, par, cycle);  // добавляем цикл в вектор cycle
        }
        
        if (used[v.first] == 0) {  // если ребро в "белую" вершину, то просто запускаем dfs
            par[v.first] = {u, v.second};  // не забывая предка вершины запомнить в векторе и стоимость ребра из него
            dfs(v.first, new_col, g, used, par, cycle);
        }
    }

    return;
}

//map <pair <int, int>, int> num_edges;
vector <long long> c(int(2e5)+10);
void dfs_set_power(int u, long long val, vector <char> &used, vector <vector <pair <int, int>>> &g, vector <long long> &powers) {
    powers[u] = val;
    used[u] = 1;

    for (auto v: g[u]) {
        if (used[v.first] == 0) {
            dfs_set_power(v.first, v.second - val, used, g, powers);
        }
    }
}

long long get_cost(long long x, int n, vector <char> &used, vector <vector <pair <int, int>>> &g, vector <long long> &powers) {
    used.assign(n+1, 0);
    dfs_set_power(1, x, used, g, powers);

    long long cost = 0;
    for (int i = 1; i <= n; i ++)
        cost += abs(powers[i]);
    return cost;
}

long long ternar_search(int n, vector <char> &used, vector <vector <pair <int, int>>> &g, vector <long long> &powers) {
    
    long long l = -1e11, r = 1e11;  // подбираем x на отрезке [l, r) теранрным поиском
    while (r - l > 3) {
        long long a = l + (r - l) / 3;
        long long b = a + (r - l) / 3;

        if (get_cost(a, n, used, g, powers) < get_cost(b, n, used, g, powers))
            r = b;
        else
            l = a;
    }

    long long min_cost = 2e18;
    long long bestx = -1;
    for (long long x = l; x < r; x ++) {
        long long curr_cost = get_cost(x, n, used, g, powers);
        if (curr_cost < min_cost) {
            min_cost = curr_cost;
            bestx = x;
        }
    }
    return bestx;
}
            
int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);

    int n, m;
    cin >> n >> m;
    
    vector <vector <pair <int, int>>> g(n+1, vector <pair <int, int>>());
    vector <pair <int, int>> edges(m);
    for (int i = 0; i < m; i ++) {
        int ai, bi, ci;
        cin >> ai >> bi >> ci;
        g[ai].push_back({bi, ci});
        g[bi].push_back({ai, ci});
        c[i] = ci;
        //num_edges[{ai, bi}] = i;  // номер ребра по паре 
        //num_edges[{bi, ai}] = i;  // номер ребра по паре 
        edges[i] = {ai, bi};
    }
    
    vector <pair <int, int>> par(n+1);  // par[i] - номер вершины-предка для вершины i в дереве обхода dfs... (0, если нет)
    vector <char> used(n+1, 0);  // used[i] = 0, если вершина еще не посещена и 'r' или 'b' если покрашена 
    vector <pair <int, int>> cycle;
    dfs(1, 'r', g, used, par, cycle);  // ищем нечетный цикл
    assert((cycle.size() == 0) || (cycle.size() >=3 && cycle.size() % 2 == 1));

    int start = 1;  //  с какой вершины начинать обход и расставлять станции
    long long x = 0;  // сила станции в ней
    bool flag = 0;
    if (cycle.size() != 0) {
        start = cycle[0].first;

        int u;
        int v = cycle[0].first;
        long long sum = 0;

        for (int i = 1; i <= cycle.size(); i ++) {  // суммируем c[i] на рёбрах цикла (чередуем знак)
            u = v;
            
            v = cycle[i % (cycle.size())].first;
            int cost_u_v = cycle[i % (cycle.size())].second;
            //cout << "! " << u << " " << cost_u_v << endl;
            if (i % 2 == 1)
                sum += cost_u_v;
            else
                sum -= cost_u_v;
        }
        //cout << sum << endl;
        if (sum % 2 != 0) {  // чередующаяся сумма ребер в нечетном цикле равна 2x, где x - сила станции в начальной вершине цикла
            cout << "No" << endl;
            exit(0);
        } else {
            x = sum / 2;
        }
        flag = 1;  // установили flag, что только такой x
    }

    used.assign(n+1, 0);
    vector <long long> powers(n+1);
    dfs_set_power(start, x, used, g, powers);


    for (int i = 0; i < m; i ++) {
        int ai = edges[i].first;
        int bi = edges[i].second;

        if (powers[ai] + powers[bi] != c[i]) {
            cout << "No" << endl;
            return 0;
        }
    }

    if (flag) {
        cout << "Yes" << endl;
        for (int i = 1; i <= n; i ++)
            cout << powers[i] << " ";
    } else {
        cout << "Yes" << endl;
        x = ternar_search(n, used, g, powers);
        used.assign(n+1, 0);
        dfs_set_power(1, x, used, g, powers);

        for (int i = 1; i <= n; i ++)
            cout << powers[i] << " ";
    }

    

    return 0;
    
}
