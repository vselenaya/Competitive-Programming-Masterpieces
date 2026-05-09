#include <iostream>
#include <vector>
#include <queue>
#include <fstream>

/* Очень симпатичная задача, развивающая идеи из задачи 9. BFS Distances Calculation.
   P.S. Это 4-ая (из 5) задача контеста Яндекса для отбора на Стажировку (весна-лето 2025 | бэкенд).
        (на контест давалось 5 часов) */

#pragma GCC optimize ("O3")
#pragma GCC target ("sse4")
#pragma GCC optimize ("unroll-loops")

using namespace std;

const int INF = 1e6;

int main() {
    std::ios::sync_with_stdio(false);  // ускорение программы
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    //std::ifstream cin("input.txt");

    int n, m, d;
    cin >> n >> m >> d;

    vector <vector <int>> manh_dist(n+2, vector <int>(m+2, INF));  // в (i,j) будем хранить манхеттенское расстояние от 
                                                                   // клетки (i,j) до ближайшего жилого объекта (изначально
                                                                   // INF - то есть бесконечно далеко)
    queue <pair <int, int>> q;  // сюда изначально сложим все клетки с жилыми объектами -> от них разом запустим поиск в ширину
    //vector <vector <bool>> used(n+2, vector <bool>(m+2, 0));  // использована ли клетка в поиске в ширину

    for (int i = 1; i <= n; ++i) {  // читаем вход
        for (int j = 1; j <= m ; ++j) {
            char c;
            cin >> c;
            if (c == 'x') {
                q.push({i, j});
                manh_dist[i][j] = 0;
                //used[i][j] = 1;
            }
        }
    }

    //int ccc = 0;

    while (!q.empty()) {  // осуществляем поиск в ширину от всех жилых объектов -> заполняем матрицу manh_dist
        auto p = q.front();
        q.pop();
        int i = p.first;
        int j = p.second;

        //ccc++;
        //if (ccc % 1000000 == 0)
            //cout << ccc << endl;
    
        const int di[] = {-1, 0, 1, 0};
        const int dj[] = {0, -1, 0, 1};
    
        for (int k = 0; k < 4; k++) {
            int ni = i + di[k];
            int nj = j + dj[k];
            if (1 <= ni && ni <= n && 1 <= nj && nj <= m && manh_dist[ni][nj] == INF) {
                //used[ni][nj] = 1;
                q.push({ni, nj});
                manh_dist[ni][nj] = manh_dist[i][j] + 1;
            }
        }
    }
    //cout << "!" << endl;

    //used.assign(n+2, vector <bool>(m+2, 0));  // очищаем вектор, теперь он будет использоваться для следующего поиска 
    vector <vector <int>> square_dist(n+2, vector <int>(m+2, INF));  // в (i,j) храним расстояние (минимальное из) от всех 
                                                                     // недоступных клеток (это границы матрицы + клетки
                                                                     // на <=d от жилых); причем тут расстояние квадратное
                                                                     // (то есть от клетки все её 8 соседей вокруг 
                                                                     // расположены на расстоянии 1 -> у такого
                                                                     // расстояния линии уровня - квадраты (у манхеттенского
                                                                     // ромбы...))
    for (int i = 0; i <= n+1; ++i) {
        for (int j = 0; j <= m+1; ++j) {
            if (i == 0 || j == 0 || i == n+1 || j == m+1 || manh_dist[i][j] < d) {
                //used[i][j] = 1;
                square_dist[i][j] = 0;
                q.push({i, j});  // складываем недоступные клетки -> от них запустим поиск по квадратному расстоянию
            }
        }
    }

    while (!q.empty()) {  // осуществляем поиск в ширину -> заполняем матрицу square_dist
        auto p = q.front();
        q.pop();
        int i = p.first;
        int j = p.second;

        for (int di = -1; di <= 1; ++di) {
            for (int dj = -1; dj <= 1; ++dj) {
                // клетки (i+di, j+dj) - все клетки на квадратном расстоянии 1 от клетки (i,j)
                if (1 <= i+di && i+di <= n && 1 <= j+dj && j+dj <= m && square_dist[i+di][j+dj] == INF) { 
                    q.push({i+di, j+dj});
                    square_dist[i+di][j+dj] = square_dist[i][j] + 1;
                }
            }
        }
    }

    int ans = 0;
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            ans = max(ans, 2 * square_dist[i][j] - 1);  // square_dist[i][j] - макс радиус квадрата c центром в (i,j)
            if (i < n && j < m &&\
                square_dist[i][j] == square_dist[i+1][j] && square_dist[i][j] == square_dist[i][j+1] &&\
                square_dist[i][j] == square_dist[i+1][j+1])  // если квадрат 2 на 2 (с левым верхним углом в (i,j)) обладает
                                                             // одинаковыми значениями, можно сделать квадрат четной длины,
                                                            // где этот 2на2 будет в центре
                ans = max(ans, 2 * square_dist[i][j]);
        }
    }
    cout << ans << endl;
}