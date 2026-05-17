#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <vector>

/* То же решение, но немного иначе написанное, чтобы избежать создание map внутри рекурсии.
Работает и правда сильно быстрее -- максимум по всем тестам: 171ms (время) и 3.99Mb (память). */

using namespace std;
using u64 = uint64_t;

// Рекурсивная функция конструирования пар (без копирований и map)
void solve(int l, int r, u64 mask, const vector<u64>& x, const vector<int>& ids, vector<u64>& ans) {
    // Базовый случай: пустой интервал или один элемент
    if (l >= r - 1) {
        if (l == r - 1) ans[ids[l]] = 0;
        return;
    }

    // Находим максимальное значение среди доступных битов (mask)
    // Т.к. ids отсортированы по убыванию x, x[ids[l]] содержит старший активный бит
    u64 max_val = x[ids[l]] & mask;
    
    // Если оставшиеся биты — нули, ответы для этих чисел в данном поддереве — нули
    if (max_val == 0) {
        for (int i = l; i < r; ++i) {
            ans[ids[i]] = 0;
        }
        return;
    }

    // Определяем старший бит
    int p = 63 - __builtin_clzll(max_val);
    u64 current_bit = 1ULL << p;
    u64 next_mask = mask ^ current_bit; // Сбрасываем этот бит для дочерних вызовов

    // Ищем границу разделения на S1 (с битом p) и S0 (без бита p)
    // Массив отсортирован по убыванию, поэтому числа с битом p идут строго в начале
    int mid = l;
    while (mid < r && (x[ids[mid]] & current_bit)) {
        mid++;
    }

    // Рекурсивно собираем ответы для подмножеств
    solve(l, mid, next_mask, x, ids, ans); // Ветка S1'
    solve(mid, r, next_mask, x, ids, ans); // Ветка S0

    // Магия биекции: слияние результатов
    for (int i = l; i < mid; ++i) {
        int id_u = ids[i];
        u64 u = x[id_u];
        u64 v = u ^ current_bit; // Значение "младшего брата" из множества S0

        // Ищем брата в правой половине (S0) бинарным поиском
        auto it = lower_bound(ids.begin() + mid, ids.begin() + r, v, 
            [&x](int id, u64 val) {
                return x[id] > val; // Кастомный компаратор для сортировки по убыванию
            });
        
        int id_v = ids[distance(ids.begin(), it)]; // Извлекаем индекс брата

        // Применяем 3 правила биекции прямо по индексам:
        u64 y = ans[id_u];            // 1. Запоминаем локальный ответ для u из подзадачи S1'
        ans[id_u] = ans[id_v];        // 2. Старший брат забирает текущий ответ младшего
        ans[id_v] = y | current_bit;  // 3. Младший брат получает ответ старшего с установленным битом
    }
}

int main() {
    // Ускоряем ввод-вывод
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    vector<u64> x(n);
    for (int i = 0; i < n; ++i) {
        cin >> x[i];
    }

    // Создаем массив индексов и сортируем ЕГО, а не сами значения
    // Это позволит мгновенно собирать ответы в изначальном порядке
    vector<int> ids(n);
    iota(ids.begin(), ids.end(), 0); // Заполняем: 0, 1, 2... n-1
    sort(ids.begin(), ids.end(), [&](int a, int b) {
        return x[a] > x[b];
    });

    vector<u64> ans(n);
    
    // Запускаем рекурсию (маска изначально состоит из всех единиц)
    solve(0, n, ~0ULL, x, ids, ans);

    // Выводим результат! (Он уже сформирован в правильном порядке)
    for (int i = 0; i < n; ++i) {
        cout << ans[i] << '\n';
    }

    return 0;
}
