/*
========================================================================================
АЛГОРИТМ ПОСТРОЕНИЯ ЖЁСТКОГО МАГИЧЕСКОГО КВАДРАТА (Концентрического)
========================================================================================

Поскольку размер квадрата N всегда нечётный (N = 2k + 1), мы можем использовать
строгий математический "метод окаймления". Мы строим квадрат послойно, от центра к краям.

1. Центральный элемент:
   Самая центральная клетка всегда получает значение (N^2 + 1) / 2.

2. Послойное окаймление:
   Для каждого слоя d (от 1 до k) мы добавляем рамку к уже построенному внутреннему 
   квадрату. Размер рамки: (2d + 1) x (2d + 1).
   Чтобы квадрат оставался магическим на каждом слое, мы используем для рамки
   симметричные пары чисел: самое маленькое из доступных + самое большое из доступных.
   Сумма любой пары всегда равна M_global = N^2 + 1.

3. Разбиение чисел для слоя d:
   Нам нужно 4d пар чисел. Мы берем 4d наименьших из оставшихся и 4d наибольших.
   Обозначим локальные "маленькие" числа от 1 до 4d.
   Нам нужно распределить эти пары по верхней (T), нижней (B), левой (L) и правой (R)
   сторонам рамки. 
   Чтобы суммы сошлись, противоположные клетки в рамке всегда содержат числа из одной пары 
   (если сверху маленькое x, то снизу строго большое M_global - x).

4. Выбор конкретных чисел для сторон:
   Нам нужно выбрать:
   - 2 угловые клетки для верхней строки (мы фиксируем их как 2d+2 и 2d).
   - (d - 1) элементов для оставшейся верхней строки из пула S' = {2d+1, ..., 4d} \ {2d+2}.
   - d элементов для левого столбца из пула L_pool = {1, 2, ..., 2d-1}.
   
   Математическое условие магичности рамки сводится к тому, что разность сумм 
   выбранных элементов для верхней строки и левого столбца должна строго равняться 
   константе (2d^2 - 2d - 1). 
   Поскольку пулы чисел почти непрерывны, мы можем легко найти подходящие подмножества
   жадным алгоритмом, просто перебрав возможную сумму для одной из сторон!

5. Трансляция в глобальные числа:
   Локальное число x (от 1 до 4d) легко переводится в глобальное добавлением `offset`,
   поскольку каждый внешний слой забирает строго известное количество чисел.

В результате мы получаем $O(N^2)$ алгоритм, который собирает идеальный жёсткий 
магический квадрат без сложных переборов и возвратов!
========================================================================================
*/

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;

// Функция для жадного поиска подмножества размера K с заданной суммой.
// Т.к. наши пулы чисел (pool) представляют собой почти непрерывные последовательности,
// этот жадный подход находит решение моментально.
vector<int> get_subset_greedy(const vector<int>& pool, int k, int target) {
    if (k == 0) return target == 0 ? vector<int>() : vector<int>{-1};
    
    int n = pool.size();
    vector<int> idx(k);
    int sum = 0;
    for (int i = 0; i < k; ++i) {
        idx[i] = i;
        sum += pool[i];
    }
    
    if (sum > target) return {-1};
    
    int right_limit = n - 1;
    for (int i = k - 1; i >= 0; --i) {
        while (idx[i] < right_limit && sum - pool[idx[i]] + pool[idx[i] + 1] <= target) {
            sum = sum - pool[idx[i]] + pool[idx[i] + 1];
            idx[i]++;
        }
        right_limit = idx[i] - 1;
    }
    
    if (sum == target) {
        vector<int> res(k);
        for (int i = 0; i < k; ++i) res[i] = pool[idx[i]];
        return res;
    }
    return {-1};
}

// Перевод локального маленького числа рамки слоя d в глобальное число N*N
int get_global(int local_x, int d, int N) {
    int k = N / 2;
    int offset = 2 * (k * (k + 1) - d * (d + 1));
    return offset + local_x;
}

// Перевод локального большого числа рамки (комплемента) слоя d в глобальное
int get_global_large(int local_x, int d, int N) {
    int small_val = get_global(local_x, d, N);
    return N * N + 1 - small_val;
}

// Основная функция генерации жёсткого магического квадрата
vector<vector<int>> generate_rigid_magic_square(int N) {
    vector<vector<int>> A(N, vector<int>(N, 0));
    int k = N / 2;
    
    // Центр
    A[k][k] = (N * N + 1) / 2;

    // Идём по слоям от 1 (внутренний) до k (внешний)
    for (int d = 1; d <= k; ++d) {
        int x0 = 2 * d + 2;
        int x2d = 2 * d;
        int target_diff = 2 * d * d - 2 * d - 1;

        vector<int> S_prime;
        for (int x = 2 * d + 1; x <= 4 * d; ++x) {
            if (x != 2 * d + 2) S_prime.push_back(x);
        }
        
        vector<int> L_pool;
        for (int x = 1; x <= 2 * d - 1; ++x) {
            L_pool.push_back(x);
        }

        int min_S = 0, max_S = 0;
        if (d - 1 > 0) {
            for (int i = 0; i < d - 1; ++i) min_S += S_prime[i];
            for (int i = 0; i < d - 1; ++i) max_S += S_prime[S_prime.size() - 1 - i];
        }

        int min_L = 0, max_L = 0;
        if (d > 0) {
            for (int i = 0; i < d; ++i) min_L += L_pool[i];
            for (int i = 0; i < d; ++i) max_L += L_pool[L_pool.size() - 1 - i];
        }

        int start_sum_S = max(min_S, min_L + target_diff);
        int end_sum_S = min(max_S, max_L + target_diff);

        vector<int> ST_prime, LT;
        for (int sum_S = start_sum_S; sum_S <= end_sum_S; ++sum_S) {
            int sum_L = sum_S - target_diff;
            
            ST_prime = get_subset_greedy(S_prime, d - 1, sum_S);
            if (!ST_prime.empty() && ST_prime[0] == -1) continue;

            LT = get_subset_greedy(L_pool, d, sum_L);
            if (!LT.empty() && LT[0] == -1) continue;

            // Нашли подходящие подмножества!
            break; 
        }

        // Оставшиеся числа идут в дополняющие стороны
        vector<bool> in_ST_prime(4 * d + 1, false);
        for (int x : ST_prime) in_ST_prime[x] = true;
        
        vector<int> SL;
        for (int x : S_prime) {
            if (!in_ST_prime[x]) SL.push_back(x);
        }

        vector<bool> in_LT(4 * d + 1, false);
        for (int x : LT) in_LT[x] = true;
        
        vector<int> LL;
        for (int x : L_pool) {
            if (!in_LT[x]) LL.push_back(x);
        }

        // Заполняем массивы верхней строки (T) и левого столбца (L_col)
        vector<int> T_arr(2 * d + 1);
        T_arr[0] = get_global(x0, d, N);
        T_arr[2 * d] = get_global(x2d, d, N);
        
        int idx = 1;
        for (int x : ST_prime) T_arr[idx++] = get_global(x, d, N);
        for (int x : LT)       T_arr[idx++] = get_global_large(x, d, N);

        vector<int> L_arr(2 * d - 1);
        idx = 0;
        for (int x : SL) L_arr[idx++] = get_global(x, d, N);
        for (int x : LL) L_arr[idx++] = get_global_large(x, d, N);

        // Размещаем всё в глобальной матрице
        int r_top = k - d;
        int r_bot = k + d;
        int c_left = k - d;
        int c_right = k + d;

        // Углы
        A[r_top][c_left]  = T_arr[0];
        A[r_top][c_right] = T_arr[2 * d];
        A[r_bot][c_left]  = N * N + 1 - T_arr[2 * d];
        A[r_bot][c_right] = N * N + 1 - T_arr[0];

        // Горизонтали
        for (int i = 1; i <= 2 * d - 1; ++i) {
            A[r_top][c_left + i] = T_arr[i];
            A[r_bot][c_left + i] = N * N + 1 - T_arr[i];
        }

        // Вертикали
        for (int i = 0; i < 2 * d - 1; ++i) {
            A[r_top + 1 + i][c_left]  = L_arr[i];
            A[r_top + 1 + i][c_right] = N * N + 1 - L_arr[i];
        }
    }

    return A;
}

int main() {
    // Вектор необходимых N (21 штука из условия)
    vector<int> ns = {5,7,9,11,25,39,53,61,99,101,135,243,343,401,493,519,611,729,841,955,999};
    // for (int i = 1; i <= 1000; i += 2) { // для себя можно и все до 1000 *и даже больше) сгенерировать: 1, 3, 5 ... 999
    //     ns.push_back(i);
    // }

    for (int i = 0; i < ns.size(); ++i) {
        int n = ns[i];
        vector<vector<int>> A = generate_rigid_magic_square(n);

        // Формируем имя файла (например, 005.out, 015.out -- по порядковому номеру теста, как требует условие)
        char filename[20];
        snprintf(filename, sizeof(filename), "answers/%03d.out", i+1);
        
        ofstream fout(filename);
        if (!fout) {
            cerr << "Не удалось открыть файл " << filename << "\n";
            continue;
        }

        for (int r = 0; r < n; ++r) {
            for (int c = 0; c < n; ++c) {
                fout << A[r][c] << (c == n - 1 ? "" : " ");
            }
            fout << "\n";
        }
        
        fout.close();
        cout << "Сгенерирован квадрат для N = " << n << " в файл " << filename << "\n";
    }

    cout << "Всё успешно выполнено!\n";
    return 0;
}
