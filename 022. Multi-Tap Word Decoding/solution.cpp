#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#pragma GCC optimize ("O3")
#pragma GCC target ("sse4")
#pragma GCC optimize ("unroll-loops")

/*
Ещё одна отличная задачка от Яндекса с контеста на Стажировку весна-лето 2025 | бэкенд.

Идея такая:
Давайте сопоставим каждому входному слову words[i] его код (в виде строки с номерами барабанов) codes[i].
Тогда задача сводится к тому, чтобы нарезать исходную строчку s на куски, каждый из которых явяляется каким-то codes[i] ->
-> нарезав так, мы просто выведем в ответе соответствующие words[i] на нужном месте через пробелы и всё.

Как же нарезать s? Давайте заведём список starts, где для каждого k-го символа строки s (от 0-го до s.size()-1 го) будем
хранить список пар (id, len), где id - номер строки, что codes[i] входит в s, начиная с k-ого символа, а len - длина codes[i].
Если у нас будет такой массив, то всё просто: фактически на символах строки s строится граф, где из одного символа (i-ым) в другой (j-ий) есть
переход, если начиная с первого (i-го) символа есть вхождение какой-то подстроки, оканчивающейся ровно перед вторым (j-им) символом. Таким образом,
мы хотим запустить обычный поиск в глубину (dfs) с 0го символа строки s -> и в итоге найти путь от него до конца строки, то есть до 
s.size()-го символа. Найдя путь, мы найдем последовательность подстрок, на которые нарезается s.

Осталось понять, как же посчитать starts? Я просто запускал КМП (алгоритм Кнута-Морриса-Пратта) для каждой пары codes[i] и s ->
-> получал все вхождения codes[i] как подстроки в s. Возможно, это долговато, но казалось бы, КМП работает за линию - то есть
если у нас s до 128000, codes[i] около 1000 и n до 1000, то вроде всё вычисленик starts это 130 млн шагов - вроде немного...
ну возможно стоило Ахо-Корасик написать... Вот второе решение через Бор делает быстрее.
*/

using namespace std;

// получаем i-ый символ строки s := s1#s2 (сконкатенировали строки через уникальный символ)
char get_symbol(int i, string &s1, string &s2) {
    if (i < s1.size())
        return s1[i];
    else if (i == s1.size())
        return '#';
    else 
        return s2[i - s1.size() - 1];
}

int pf[130'000];

// pf[i] - длина наибольшего (собственного) суффикса = префиксу в строке s1#s2 (# - уникальный символ)
void prefix_func(string &s1, string &s2) {  // вычисление вектора префикс-функции по строке
    pf[0] = 0;
    int len = s1.size() + 1 + s2.size();

    for (int i = 0; i < len - 1; i ++) {  // в цикле вычисляем значение префикс функции на i+1, зная уже i
        int pes = pf[i];  // длина текущего наибольшего префикса = суффикс

        while(1) {
            if (get_symbol(i+1, s1, s2) == get_symbol(pes, s1, s2)) {  // если можно продлить на i+1-ый символ префикс
                pf[i+1] = pes + 1;  // продляем
                break;
            }

            if (pes == 0) {  // если префикс 0
                pf[i+1] = 0;
                break;
            }

            pes = pf[pes-1];  // переходим к следующему по длине префиксу = суффиксу в s0, s1, ..., si
        }
    }
}

static string drum_map[256];
void encode(string &word, string &code) {
    code.clear();

    for (char c : word) {
        code += drum_map[c];
    }
}

bool dfs(int v, vector <vector <pair <int, int>>> &starts, vector <int> &p, vector<bool> &visited) {
    int s = starts.size();
    if (v > s)
        throw string("Вышли за границу!");
    if (v == s)
        return true;
    if (starts[v].size() == 0)
        return false;
    if (visited[v]) return false;  // Спасение от TL!
    visited[v] = true;

    for (auto id_len: starts[v]) {
        int u = v + id_len.second;
        p[u] = id_len.first;

        if (dfs(u, starts, p, visited))
            return true;
    }
    return false;
}

int main() {
    std::ios::sync_with_stdio(false);  // ускорение программы
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    drum_map['A'] = "2"; drum_map['B'] = "22"; drum_map['C'] = "222";
    drum_map['D'] = "3"; drum_map['E'] = "33"; drum_map['F'] = "333";
    drum_map['G'] = "4"; drum_map['H'] = "44"; drum_map['I'] = "444";
    drum_map['J'] = "5"; drum_map['K'] = "55"; drum_map['L'] = "555";
    drum_map['M'] = "6"; drum_map['N'] = "66"; drum_map['O'] = "666";
    drum_map['P'] = "7"; drum_map['Q'] = "77"; drum_map['R'] = "777"; drum_map['S'] = "7777";
    drum_map['T'] = "8"; drum_map['U'] = "88"; drum_map['V'] = "888";
    drum_map['W'] = "9"; drum_map['X'] = "99"; drum_map['Y'] = "999"; drum_map['Z'] = "9999";

    string s;
    cin >> s;

    int n;
    cin >> n;

    vector <string> words(n);
    vector <string> codes(n);
    for (int i = 0; i < n; ++i) {
        cin >> words[i];
        encode(words[i], codes[i]);
        // cout << "---> " << words[i] << ":   " << codes[i] << endl;
    }
        
    vector <vector <pair <int, int>>> starts(s.size());  // по номеру цифры храним набор пар {id, len} - номер строки, 
                                                         // код которой может тут начаться и длина этого кода
    for (int i = 0; i < n; i ++) {
        prefix_func(codes[i], s);
        int offset = codes[i].size() + 1;
        for (int d = offset; d < offset + s.size(); d ++) {
            if (pf[d] == codes[i].size()) {
                starts[d-int(codes[i].size())+1-offset].push_back({i, codes[i].size()});
            }
        }
    }

    vector <int> p(s.size()+1, -1);
    vector<bool> visited(s.size() + 1, false);
    bool res = dfs(0, starts, p, visited);

    if (!res)
        throw string("Некорректно!");

    vector <int> strs;
    int curr = s.size();
    while (curr > 0) {
        strs.push_back(p[curr]);
        curr -= codes[strs.back()].size();
    }

    reverse(strs.begin(), strs.end());
    for (int i: strs)
        cout << words[i] << " ";
}