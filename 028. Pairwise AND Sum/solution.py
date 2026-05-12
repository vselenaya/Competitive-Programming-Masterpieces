def cnt_p_bit(n, p):
    block_size = 2 ** p
    d = n // block_size
    rem = n % block_size

    if (d % 2 == 0):
        return block_size * (d // 2)
    else:
        return block_size * (d // 2) + (rem + 1)

def solve():
    n = int(input())

    ans = 0
    for p in range(64):
        cnt = cnt_p_bit(n, p)
        ans += (2 ** p) * (cnt ** 2)

    # print(ans)  # у нас сумма длины 10^18 * 10^18 чисел до 10^18 -> ответ не больше 10^54, Python может целиком считать :)
                  # например, для максимального n=10^18 полный ответ = 217487073886266806486235264116354566966769499192426496
    print(ans % (10 ** 9 + 7))

t = int(input())
for _ in range(t):
    solve()
