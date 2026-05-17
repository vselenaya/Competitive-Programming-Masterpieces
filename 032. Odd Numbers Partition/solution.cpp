#include <iostream>
#include <cassert>
#include <vector>

using namespace std;

static constexpr long long INF = 1e18;  // сумма всех чисел для n=10^5 достигает 10^10, так что берём INF побольше

/* Проверяет, можно ли разбить числа 1, 3, 5, ..., (2i-1), ..., 2n-1 на k кучек равной суммы */
bool can_partiate(int n, int k) {
    long long sum = n * 1ll * n;  // сумма таких чисел n^2, она должна делиться на число кучек (чтобы в них была равная сумма)  
    if (sum % k != 0) {
        return false;
    }
    return (sum / k) >= (2 * n - 1);  // сумма в каждой кучке будет sum/k, это должно быть не меньше максимального числа 2n-1 (иначе его некуда будет положить)
}  // оказывается, этого достаточно...

/* Функция перебора с возвратом. Она рекурсивно пытается набрать сумму traget с помощью элементов вектора vals, начиная с индекса ind.
Если удаётся, она складывает слагаемые в part и возвращает true (иначе, ничего не складывает и возвращает false). */
bool recursive(long long target, int ind, vector<long long> &vals, vector<int> &part) {
    if (target == 0)
        return true;  // если остался 0, то всё хорошо (набрать сумму 0 = не брать никаких слагаемых), уже набрали
    else if (ind >= vals.size())
        return false;  // если так и не набрали (остаётся нужно набрать > 0), но элементы vals кончились (индекс вышел за граниы), то уже не наберем

    for (int i = ind; i < vals.size(); ++i) {  // проходимся по элементам, начиная с ind
        if (target >= vals[i] && recursive(target - vals[i], i+1, vals, part)) {  // откусываем слагаемое vals[i] и рекурсивно пытаемся набрать остаток
            part.push_back(vals[i]);
            vals[i] = INF;  // если удаётся, помечаем vals[i] использованным и выходим
            return true;
        }
    }
    return false;
}

/* Функция разбивает n на k кучек и выводит их */
void print_partitiation(int n, int k) {
    assert(can_partiate(n, k));
    cout << k << '\n';

    vector<long long> vals;  // складываем все n доступных чисел для разбиения в вектор (важно, что от большего к меньшему, чтобы в recursive побыстрее набрать сумму...)
    for (int i = n; i >= 1; --i) {
        vals.push_back(2 * i - 1);
    }

    for (int p = 0; p < k; ++p) {  // по отдельности набираем каждую кучку
        vector<int> part;
        if (!recursive((n * 1ll * n) / k, 0, vals, part)) {  // стартуем с n^2/k (это целевая сумма каждой кучки, её набираем) и индекса = 0
            throw string("some error");                                     // если вдруг не получится, то что-то странное...
        }

        cout << part.size() << " ";
        for (int p : part)
            cout << p << " ";
        cout << '\n';
    }
}

int main() {
    ios_base::sync_with_stdio(false);  // Ускоряем ввод/вывод
    cin.tie(NULL);
    cout.tie(NULL);

    int n;
    cin >> n;

    for (int k = 2; k <= n; ++k) {  // проверяем, можем ли разбить на k >= 2 кучки
        if (can_partiate(n, k)) {
            print_partitiation(n, k);  // если да -- выводим
            return 0;
        }
    }

    cout << -1 << endl;  // если не получается, выводим -1
    return 0;
}
