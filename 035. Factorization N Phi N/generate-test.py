# Простенький файл для собственного тестирования. Генерируем m = n * phi(n)/2, где n = ввод * p * q (p, q --большие простые)
# а потом попытаться восстановить n через solution.py. Так как у нас код на питоне, то в целом мы не ограничены
# значениями 10^18 или 10^36 из условия задачи. Можем факторизовать и побольше значения!

import random
import math

def is_prime(n: int, miller_rabin_attempts: int) -> bool:
    """
    Детерминированный для малых и вероятностный тест Миллера-Рабина 
    для проверки числа на простоту.
    Возвращает True, если чилос простое.
    """
    if n < 2: return False
    if n in (2, 3, 5, 7, 11, 13, 17, 19, 23): return True
    if n % 2 == 0: return False
    
    r, s = 0, n - 1
    while s % 2 == 0:
        r += 1
        s //= 2
        
    for _ in range(miller_rabin_attempts):
        a = random.randint(2, n - 1)
        x = pow(a, s, n)
        if x == 1 or x == n - 1:
            continue
        for _ in range(r - 1):
            x = pow(x, 2, n)
            if x == n - 1:
                break
        else:
            return False
    return True

def generate_large_prime(digits: int) -> int:
    """Генерирует случайное простое число заданной длины."""
    lower_bound = 10**(digits - 1)
    upper_bound = 10**digits - 1
    while True:
        candidate = random.randint(lower_bound, upper_bound)
        if candidate % 2 == 0:
            candidate += 1
        if any(candidate % p == 0 for p in (3, 5, 7, 11, 13, 17, 19, 23, 29, 31)):
            continue
        if is_prime(candidate, miller_rabin_attempts=45):
            return candidate
        
def euler_phi(n: int) -> int:
    """Вычисляет функцию Эйлера для одного числа n за O(sqrt(n))."""
    if n < 1:
        return 0
    
    result = n
    p = 2
    while p * p <= n:
        if n % p == 0:
            # p — простой делитель, обновляем результат
            while n % p == 0:
                n //= p
            result -= result // p
        p += 1
        
    # Если в конце осталось простое число > sqrt(n)
    if n > 1:
        result -= result // n
        
    return result

k = int(input())
p = generate_large_prime(30)
q = generate_large_prime(30)

n = p * q * k
assert (p != q) and (math.gcd(p, k) == 1) and (math.gcd(q, k) == 1)
# Считаем функцию Эйлера свойствам мультипликативности.
phi_k = euler_phi(k)
phi_n = (p - 1) * (q - 1) * phi_k
    
E = n * phi_n
assert E % 2 == 0
print(f"Ввод k = {k}")
print(f"Простое p = {p}")
print(f"Простое q = {q}")
print(f"Число (ответ) n = k * p * q = {n}")
print(f"Число (тест) m = {E // 2}")
