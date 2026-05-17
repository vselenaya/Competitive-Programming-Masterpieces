#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <vector>
#include <unordered_map>

/* Это решение, которое возможно проще придумывается, но про него ничего сказать не могу, почему оно работает...
Идея жадная: давайте идти по числам, начиная с самых "капризных", то есть у которых больше всего установленных бит
(им сложнее найти пару, что & давал 0). Для каждого из таких x будем искать пару y тоже с наибольшим число установленных
бит (чтобы такие капризные побыстрее использовать), что x & y = 0. Простейшая реализация -- для каждого x из n чисел ищем по 
всем n пару y. Но это O(n^2), что получает Time Limit. Поэтому мы бы хотели уметь быстро искать для кажого числа x число y
с наибольшим числом установленных битов. Как это сделать гарантированно быстро -- не знаю, но можно попробовать так: сложим
числа в бор (двоично дерево с ребрами, помеченными 0 или 1 -> путь до листа соответствует битовой записи числа). Далее для
интересующего нас x будем спускаться по этому бору: если очередной бит x равен 1, то мы обязаны пойти по ветке с битом 0 (чтобы
& давало 0). Если очередной бит x равен 0, то можем пойти по обоим веткам -- предпочтительнее по 1 (чтобы найти число с наибольшим
количеством единиц), но возможно там будет тупик -- придется откатиться назад. В теории таких откатов будет немного...
В общем, непонятно почему код работает (видимо, если решение вообще существует, то жадный выбор "самого тяжелого" элемента
(с наибольшим количеством бит) не портит возможность достроить ответ.) и непонятно, почему быстро.
В общем, код получает Accepted и тратит 0.527s (время) и 46.40Mb (память). */
using namespace std;
using u64 = uint64_t;

// Узел битового бора
struct Node {
    int ch[2] = {-1, -1}; // Индексы детей (0 и 1)
    int count = 0;        // Сколько доступных чисел в поддереве
};

vector<Node> trie;

// Добавляем число в бор
void add(u64 val) {
    int curr = 0;
    for (int i = 63; i >= 0; --i) {
        trie[curr].count++;
        int bit = (val >> i) & 1;
        if (trie[curr].ch[bit] == -1) {
            trie[curr].ch[bit] = trie.size();
            trie.emplace_back();
        }
        curr = trie[curr].ch[bit];
    }
    trie[curr].count++;
}

// Удаляем число из бора
void remove(u64 val) {
    int curr = 0;
    for (int i = 63; i >= 0; --i) {
        trie[curr].count--;
        int bit = (val >> i) & 1;
        curr = trie[curr].ch[bit];
    }
    trie[curr].count--;
}

// Рекурсивный поиск с бэктрекингом
// Возвращает true, если пара найдена, и записывает её в partner
bool dfs(int curr, u64 x, int bit_pos, u64& partner) {
    // Базовый случай: мы успешно дошли до конца числа
    if (bit_pos < 0) {
        return true; 
    }

    int bit = (x >> bit_pos) & 1;

    if (bit == 1) {
        // У числа X здесь 1. Мы ОБЯЗАНЫ идти только в ветку 0.
        int next_node = trie[curr].ch[0];
        if (next_node != -1 && trie[next_node].count > 0) {
            // Пробуем спуститься. Если там нашли ответ — отлично!
            return dfs(next_node, x, bit_pos - 1, partner);
        }
        // Иначе — это тупик. Возвращаем false, чтобы алгоритм откатился.
        return false; 
    } 
    else {
        // У числа X здесь 0. Мы МОЖЕМ идти и в 1, и в 0.
        
        // ЖАДНОСТЬ: Сначала пробуем пойти в 1 (чтобы забрать число с бОльшим кол-вом бит)
        int next_node_1 = trie[curr].ch[1];
        if (next_node_1 != -1 && trie[next_node_1].count > 0) {
            if (dfs(next_node_1, x, bit_pos - 1, partner)) {
                // Если путь успешен, добавляем этот бит в ответ и возвращаем true
                partner |= (1ULL << bit_pos);
                return true;
            }
        }
        
        // БЭКТРЕКИНГ: Если ветка 1 не подошла (или её вообще нет), 
        // не отчаиваемся и пробуем пойти в ветку 0!
        int next_node_0 = trie[curr].ch[0];
        if (next_node_0 != -1 && trie[next_node_0].count > 0) {
            if (dfs(next_node_0, x, bit_pos - 1, partner)) {
                // Бит 0 добавлять не нужно, он и так 0
                return true;
            }
        }

        // Если обе ветки завели в тупик — значит для текущего префикса решений нет
        return false;
    }
}

int main() {
    // Ускоряем потоки
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;

    vector<u64> x(n);
    for (int i = 0; i < n; ++i) cin >> x[i];

    // Сортируем: сначала самые "тяжелые" (больше всего единиц)
    // Если единиц поровну — сортируем по значению
    vector<u64> sorted_x = x;
    sort(sorted_x.begin(), sorted_x.end(), [](u64 a, u64 b) {
        int p1 = __builtin_popcountll(a);
        int p2 = __builtin_popcountll(b);
        if (p1 != p2) return p1 > p2;
        return a > b; 
    });

    // Создаем корень бора и закидываем все числа
    trie.emplace_back();
    for (u64 val : x) add(val);

    unordered_map<u64, u64> pair_map;
    unordered_map<u64, int> available_counts;
    for (u64 val : x) available_counts[val]++;

    for (u64 val : sorted_x) {
        // Если это число уже спарили с кем-то другим, пропускаем
        if (available_counts[val] == 0) continue;

        // Временно "изымаем" число из доступа, чтобы не спарить его с самим собой 
        remove(val);
        available_counts[val]--;

        u64 partner = 0;
        // Запускаем наш умный поиск с откатами
        bool found = dfs(0, val, 63, partner);

        if (found && available_counts[partner] > 0) {
            // Ура, пара найдена!
            pair_map[val] = partner;
            pair_map[partner] = val; // Сохраняем взаимность
            
            // Забираем партнера из бора
            remove(partner);
            available_counts[partner]--;
        } else {
            // Если чуда не произошло, возвращаем число в пул
            add(val);
            available_counts[val]++;
        }
    }

    // Выводим результат в исходном порядке
    for (int i = 0; i < n; ++i) {
        cout << pair_map[x[i]] << "\n";
    }

    return 0;
}
