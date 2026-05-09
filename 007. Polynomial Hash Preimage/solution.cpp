#include <iostream>
#include <string>
#include <map>

using namespace std;

int fast_power(long long base, int power, int mod) {
    long long result = 1 % mod;
    while (power > 0) {
        if (power & 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        power >>= 1;
    }
    return (int) result;
}

int main() {
    int q, p, h;
    cin >> q >> p >> h;
    
    if ((h >= 97) & (h <= 122)) {
        cout << char(h);
        return 0;
    }
    
    map <int, string> prefix;
    for (int i = 97; i <= 122; i ++) {
        for (int j = 97; j <= 122; j ++) {
            string temp = "00";
            temp[0] = char(i);  // склеиваем два символа в строку..
            temp[1] = char(j);
            prefix[(i + p * j) % q] = temp;
        }
    }
            
    string str = "";
    long long current_hash = 0;
    
    for (int i = 0; i <= 999998; i++) {
        int letter = rand() % 26 + 97;  // rand() % 26 - даёт случайно число от 0 до 25 -> +97 - получаем случайный ascii код буквы
        str += char(letter);
        current_hash += fast_power(p, i + 2, q) * letter;
        current_hash %= q;
        for (auto j: prefix) {
            if ((j.first + current_hash) % q == h) {  // не забываем взять по модулю q
                cout << j.second + str;
                return 0;
            }
        }
    }

    return 0;
}
