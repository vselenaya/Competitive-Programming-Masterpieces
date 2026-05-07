import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;


public class J {
    public static long fast_power(long base, int power, int mod) {  // long для надёжности, что не будет переполнения
        long result = 1;
        while (power > 0) {
            if ((power & 1) == 1) {
                result = (result * base) % mod;
            }
            base = (base * base) % mod;
            power >>= 1;
        }
        return result;
    }

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        int q, p, h;
        q = scanner.nextInt();
        p = scanner.nextInt();
        h = scanner.nextInt();


        if ((h >= 97) & (h <= 122)) {
            System.out.println((char) h);
            System.exit(0);
        }

        Map<Integer,String> prefix = new HashMap<Integer, String>();
        for (int i = 97; i <= 122; i ++) {
            for (int j = 97; j <= 122; j ++) {
                prefix.put((i + p * j) % q, Character.toString((char) i) + Character.toString((char) j));
            }
        }

        String str = "";
        long current_hash = 0;

        for (int i = 0; i < 999999; i++) {
            long letter = 97 + (int) (Math.random() * 25);  // Math.random() - случайное(действительное) число от 0 до 1
            str += Character.toString((char) letter);
            current_hash += (fast_power(p, i + 2, q) * letter);
            current_hash %= q;
            for (int j: prefix.keySet()) {
                if (((long)j + current_hash) % q == h) {
                    System.out.println(prefix.get(j) + str);
                    System.exit(0);
                }
            }
        }
    }
}

/*
замечу, что решение на C++ получает Accepted стабильнее, чем решение на Java (из двух попыток, C++ получил
Accepted оба раза, а Java из 5 попыток получила Accepted только два раза (в остальные попытки - TimeLimit
(видимо неудачно выбирались случайные буквы); при этом на Java11 задача сдвалась оба раза,
и еще два раза получила TimeLimit на 47 и 48 тесте; на Java8 же, задача получила TimeLimit на 12 тесте -
видимо, он медленннее)
 */