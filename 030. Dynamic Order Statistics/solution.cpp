/*
    ИДЕЯ РЕШЕНИЯ (SQRT-декомпозиция для динамического массива)

    1. Структура данных:
       Разобьем массив на блоки (примерно по B = sqrt{N} ~= 400 элементов). 
       Каждый блок хранит:
       - `a`: обычный вектор (его блок) элементов в их реальном порядке.
       - `sorted_a`: те же элементы, но отсортированные по возрастанию.

    2. Ответ на запрос `? L R x` (Количество чисел <= x):
       - Находим блоки, в которые попадают индексы L и R.
       - Если L и R попали в один блок, просто идем циклом от L до R внутри этого блока.
         (так как размер блока sqrt{N}, то это занимает O(sqrt{N}) времени)
       - Если блоки разные:
         - Левый "хвостик" (от L до конца левого блока) проходим циклом.
         - Правый "хвостик" (от начала правого блока до R) проходим циклом.
         - Все строго ПРОМЕЖУТОЧНЫЕ блоки покрыты запросом целиком! 
           Для них мы берем вектор `sorted_a` и за O(log B) делаем бинарный поиск
           (std::upper_bound), чтобы моментально узнать, сколько там чисел <= x.
         - Оба итерирования по "хвостикам" занимает O(sqrt{N}), так как именно такой размер блоков,
           промежуточных блоков O(sqrt{N}) (у нас весь массив N поделён на всего sqrt{N} блоков по sqrt{N}
           элементов), бинарный поиск в каждом занимает O(log(sqrt{N})) = O(log N). Итого запрос O(sqrt{N} log{N})

    3. Вставка `+ i x`:
       - Находим нужный блок.
       - Вставляем элемент `x` в вектор `a` на нужную позицию (это занимает O(B)).
       - Вставляем элемент `x` в нужное по порядку место в `sorted_a` (тоже O(B)).
       - ВАЖНО: Если из-за вставок блок слишком сильно разросся (стал больше 2 * BLOCK_SIZE), 
         мы разрезаем его на два ровных блока, чтобы операции не замедлялись.

    4. Удаление `- i`:
       - Находим блок и удаляем элемент из `a` и из `sorted_a` за O(B).
       - Если блок стал полностью пустым, просто удаляем его из списка блоков.

    Сложность: 
    Запрос: O(sqrt(N) * log(sqrt(N)))
    Обновление: O(sqrt(N))
*/

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int BLOCK_SIZE = 400; // Оптимальный размер блока (примерно корень из N=100000)

struct Block {
    vector<int> a;
    vector<int> sorted_a;

    // Перестроение отсортированного массива (нужно при разделении блока) внутри блока
    void build() {
        sorted_a = a;
        sort(sorted_a.begin(), sorted_a.end());
    }

    // Вставка элемента внутрь блока за O(B)
    void insert(int pos, int val) {
        a.insert(a.begin() + pos, val);
        // Чтобы не пересортировывать весь массив, находим позицию и вставляем умно (через бин поиск)
        auto it = upper_bound(sorted_a.begin(), sorted_a.end(), val);
        sorted_a.insert(it, val);
    }

    // Удаление элемента из блока за O(B)
    void erase(int pos) {
        int val = a[pos];
        a.erase(a.begin() + pos);
        // Находим этот же элемент в отсортированном массиве и удаляем (размуеется, считаем, что такой есть)
        auto it = lower_bound(sorted_a.begin(), sorted_a.end(), val);
        sorted_a.erase(it);
    }

    // Подсчёт элементов <= x простым перебором (будем запскать для частичных (краевых) блоков), индексы [l, r]
    int query_part(int l, int r, int x) {
        int count = 0;
        for (int i = l; i <= r; ++i) {
            if (a[i] <= x) count++;
        }
        return count;
    }

    // Подсчёт элементов <= x во всём блоке через Бинарный поиск (для полностью покрытых блоков)
    int query_full(int x) {
        auto it = upper_bound(sorted_a.begin(), sorted_a.end(), x);
        return distance(sorted_a.begin(), it);
    }
};

struct DynamicArray {
    vector<Block> blocks;

    // Поиск номера блока (b) и позиции внутри блока (p) по глобальному индексу i
    void get_pos(int i, int& b, int& p) {
        for (b = 0; b < blocks.size(); ++b) {
            if (i < blocks[b].a.size()) {
                p = i;
                return;
            }
            i -= blocks[b].a.size();
        }
        // Если индекс указывает в самый конец массива (то есть после всех элементов)
        if (!blocks.empty()) {
            b = blocks.size() - 1;
            p = blocks[b].a.size();
        } else {
            b = 0;
            p = 0;
        }
    }

    // Разделение большого блока на два, чтобы сохранить асимптотику
    void split(int b) {
        if (blocks[b].a.size() > 2 * BLOCK_SIZE) {
            Block new_block;
            int mid = blocks[b].a.size() / 2;
            
            // Перекидываем вторую половину в новый блок
            new_block.a.assign(blocks[b].a.begin() + mid, blocks[b].a.end());
            blocks[b].a.erase(blocks[b].a.begin() + mid, blocks[b].a.end());
            
            // Перестраиваем оба блока
            blocks[b].build();
            new_block.build();
            
            blocks.insert(blocks.begin() + b + 1, new_block);
        }
    }

    // вставка элемента x на позицию i 
    void insert(int i, int x) {
        if (blocks.empty()) {
            blocks.push_back(Block());
        }
        int b, p;
        get_pos(i, b, p);
        blocks[b].insert(p, x);
        split(b);
    }

    // удаление элемента i
    void erase(int i) {
        if (blocks.empty()) return;
        int b, p;
        get_pos(i, b, p);
        blocks[b].erase(p);
        
        // Удаляем пустые блоки, чтобы не плодить мусор
        if (blocks[b].a.empty()) {
            blocks.erase(blocks.begin() + b);
        }
    }

    int query(int L, int R, int x) {
        if (blocks.empty() || L > R) return 0;
        
        int b_L, p_L, b_R, p_R;
        get_pos(L, b_L, p_L);
        get_pos(R, b_R, p_R);

        if (b_L == b_R) {
            // Запрос полностью попал внутрь одного блока
            return blocks[b_L].query_part(p_L, p_R, x);
        } else {
            int ans = 0;
            // Считаем левый хвост
            ans += blocks[b_L].query_part(p_L, blocks[b_L].a.size() - 1, x);
            // Считаем правый хвост
            ans += blocks[b_R].query_part(0, p_R, x);
            // Считаем целиковые промежуточные блоки за бинпоиск!
            for (int i = b_L + 1; i < b_R; ++i) {
                ans += blocks[i].query_full(x);
            }
            return ans;
        }
    }
};

int main() {
    // Ускоряем ввод/вывод
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;

    DynamicArray arr;
    for (int i = 0; i < n; ++i) {
        int val;
        cin >> val;
        arr.insert(i, val);
    }

    char type;
    while (cin >> type) {
        if (type == '+') {
            int i, x;
            cin >> i >> x;
            arr.insert(i, x);
        } else if (type == '-') {
            int i;
            cin >> i;
            arr.erase(i);
        } else if (type == '?') {
            int L, R, x;
            cin >> L >> R >> x;
            cout << arr.query(L, R, x) << "\n";
        }
    }

    return 0;
}
