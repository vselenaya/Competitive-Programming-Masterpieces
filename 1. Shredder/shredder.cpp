// Вспомогательный код, реализующий шредер

#include <string_view>
#include <vector>
#include <fstream>
#include <string>
#include <cassert>
#include <random>

using namespace std;

// Константы
static constexpr pair<double, double> SplitProbability = {1, 3};

// Настройка генератора (Вихрь Мерсенна)
uint32_t seed = 42;
std::mt19937 gen(seed);

vector<string_view> split_into_chunks(const string& s) {
    auto [p, q] = SplitProbability;  // вероятность разделить соседние биты = p / q
    assert (p < q);
    std::discrete_distribution<> dist({(double) p, (double) q - p});

    vector<string_view> chunks;
    size_t n = s.size();

    size_t split_pos = 0;  // предыдущая позиция (бит), перед которой было разделение
    for (size_t i = 1; i < n; ++i) {
        if (dist(gen) == 0) {  // выпало событие с вероятностью p / q -> перед i-ым битом режем строку
            chunks.emplace_back(string_view(s).substr(split_pos, i - split_pos));  // создаем "взгляд" на часть строки s без копирования
            split_pos = i;
        }
    }
    chunks.emplace_back(string_view(s).substr(split_pos, n - split_pos));  // докалдываем остаток

    return chunks;
}

int main() {
    // Входной-выходной файлы:
    ifstream input("bits.txt");
    ofstream output("bits-after-shredder.txt");

    string s;
    input >> s;

    vector<string_view> chunks = split_into_chunks(s); 
    
    for (size_t i = chunks.size() - 1; i > 0; --i) {  // равномерно перемешиваем, получая равновероятно одну из chunks.size()! перестановку
        std::uniform_int_distribution<int> dist(0, i);    // создаем распределение в диапазоне [0, i]
        std::swap(chunks[i], chunks[dist(gen)]);  
    }

    for (const auto& p : chunks) {
        output << p;
    }

    return 0;
}
