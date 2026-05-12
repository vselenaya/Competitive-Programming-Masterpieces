#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

/*
Идея этого решения:
Давай сложим все переведенные слова из словаря в Бор. А вместо dfs напишем простую одномерную динамику $O(|S| \times \text{длина слова})$.
Для каждого символа строки s мы просто будем "спускаться" по Бору вперед и смотреть, какие слова можем откусить. Это работает в
десятки раз быстрее.
*/

// Структура узла Бора
struct TrieNode {
    int next[10];
    int word_id = -1; // -1, если тут не заканчивается слово, иначе индекс слова
    TrieNode() {
        fill(begin(next), end(next), -1);
    }
};

vector<TrieNode> trie;

// Добавление кода слова в Бор
void insert(const string& code, int id) {
    int u = 0;
    for (char c : code) {
        int d = c - '0';
        if (trie[u].next[d] == -1) {
            trie[u].next[d] = trie.size();
            trie.emplace_back();
        }
        u = trie[u].next[d];
    }
    trie[u].word_id = id;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string drum_map[256];
    drum_map['A']="2"; drum_map['B']="22"; drum_map['C']="222";
    drum_map['D']="3"; drum_map['E']="33"; drum_map['F']="333";
    drum_map['G']="4"; drum_map['H']="44"; drum_map['I']="444";
    drum_map['J']="5"; drum_map['K']="55"; drum_map['L']="555";
    drum_map['M']="6"; drum_map['N']="66"; drum_map['O']="666";
    drum_map['P']="7"; drum_map['Q']="77"; drum_map['R']="777"; drum_map['S']="7777";
    drum_map['T']="8"; drum_map['U']="88"; drum_map['V']="888";
    drum_map['W']="9"; drum_map['X']="99"; drum_map['Y']="999"; drum_map['Z']="9999";

    string s;
    if (!(cin >> s)) return 0;

    int n;
    cin >> n;

    vector<string> words(n);
    trie.emplace_back(); // Создаем корень Бора

    for (int i = 0; i < n; ++i) {
        cin >> words[i];
        string code = "";
        for (char c : words[i]) code += drum_map[c];
        insert(code, i); // Кладем код в Бор
    }

    int len = s.size();
    // p[i] - индекс, ОТКУДА мы пришли в позицию i (аналог твоего p, но для ДП)
    vector<int> p(len + 1, -1);
    vector<int> word_used(len + 1, -1);
    p[0] = 0; // База: в 0-й символ мы пришли успешно

    // ДП (проход по строке)
    for (int i = 0; i < len; ++i) {
        if (p[i] == -1) continue; // Если до этой позиции добраться нельзя, пропускаем
        
        int u = 0;
        // Пробуем "откусить" префикс начиная с i-го символа
        for (int j = i; j < len; ++j) {
            int d = s[j] - '0';
            u = trie[u].next[d];
            
            if (u == -1) break; // Такого пути в словаре больше нет
            
            // Если нашли слово в словаре и в позицию j+1 мы еще не приходили
            if (trie[u].word_id != -1 && p[j + 1] == -1) {
                p[j + 1] = i; // Запоминаем, откуда пришли
                word_used[j + 1] = trie[u].word_id; // Какое слово использовали
            }
        }
    }

    // Восстановление ответа с конца в начало
    vector<int> ans;
    int curr = len;
    while (curr > 0) {
        ans.push_back(word_used[curr]);
        curr = p[curr];
    }

    reverse(ans.begin(), ans.end());
    for (int i = 0; i < ans.size(); ++i) {
        cout << words[ans[i]] << (i + 1 == ans.size() ? "" : " ");
    }
    cout << "\n";

    return 0;
}
