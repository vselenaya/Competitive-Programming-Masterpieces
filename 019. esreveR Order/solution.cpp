#include <bits/stdc++.h>

using namespace std;

#define u64 uint64_t

// переворачиваем байты (не биты!), то есть меняем endian (с little на big или наоборот)
u64 rev(u64 x) {
    uint8_t *ptr = (uint8_t*) &x;  // берём указатель на кусочек 8 бит (= 1 байт) из всего числа x
    u64 ans = 0;
    for (int i = 0; i <= 7; ++i) {  // двигаемся по этим байтам
        ans *= 256;
        ans += *(ptr + i);  // добавляем байты в ответ в обратном порядке
    }
    return ans;
}

// те 24 дополнительных числа, через которые будем передавать информацию о требуемых для передачи 1000 чисел
vector<u64> S(24, 1);

// сохраняем биты (записанные как true (= 1) или false (= 0) в векторе) в эти 24 числа
void set_bits(vector <bool> &bits) {
    for (int i = 0; i < bits.size(); ++i) {
        if (bits[i])
            S[i / 48] ^= (u64(1) << (i % 48 + 8));  // для сохранения используем средние 48 бит каждого S[i]
    }                                               // (первые 8 бит будут нулевыми, последние 8 бит хранят одно число = 1, которым инициализируем ->
                                                    // -> так сможем понять, перевернулся ли сам S[i] или нет)
}

// достаём биты из S[]:
vector<bool> get_bits() {
    for (int i = 0; i < S.size(); ++i) {
        if (S[i] % 2 != 1) {  // если S[i] не оканчивается на 1, значит байты S[i] перевернулись -> переворачиваем обратно
            S[i] = rev(S[i]);
        }
    }

    vector<bool> ans;
    for (int i = 0; i < S.size(); ++i) {
        for (int j = 8; j <= 55; ++j) {  // средние 48 бит (с 8го по 55) кладём в вектор как bool
            ans.push_back((S[i] & (u64(1) << j)) > 0);
        }
    }

    return ans;
}

int main() {
    string s;
    cin >> s;

    if (s.compare("encode") == 0) {
        int n;
        cin >> n;

        vector<u64> a(n);
        for (int i = 0; i < n; ++i)
            cin >> a[i];
        
        vector<bool> bits;
        for (int i = 0; i < n; ++i) {
            bits.push_back(rev(a[i]) >= a[i]);  // о каждом числе (требуемом для передачи) запоминаем 1 бит информации: больше
        }                                            // ли число в перевёрнутом виде, чем в исходном? -> эту информацию и сохраняем в S[]

        set_bits(bits);
        cout << n + 24 << endl;
        for (int i = 0; i < n; ++i)
            cout << a[i] << " ";
        for (int i = 0; i < 24; ++i)
            cout << S[i] << " ";
    } else {
        int n;
        cin >> n;
        assert(n > 24);

        n -= 24;
        vector<u64> a(n);
        for (int i = 0; i < n; ++i)
            cin >> a[i];
        for (int i = 0; i < 24; ++i)
            cin >> S[i];

        vector<bool> bits = get_bits();
        //assert (n == bits.size());

        for (int i = 0; i < n; ++i) {
            if ((rev(a[i]) >= a[i]) == bits[i]) {  // выводим число или перевёрнутое
                cout << a[i] << " ";
            } else {
                cout << rev(a[i]) << " ";
            }
        }
    }

    cout << endl;
}
