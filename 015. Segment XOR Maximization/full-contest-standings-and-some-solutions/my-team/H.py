t = int(input())

def solve(i, t1, t2, t3, t4, t5, t6):
    #print("i -> ", i)
    if i % 2 == 0:
        return min(t1 * i, t4 * i, t5 * (i // 2))
    else:
        return min(t5 * (i//2) + min(t1, t4), min(t3, t2) + t5 * ((i+1) // 2), t1 * i, t4 * i)
   
for i in range(t):
    #print(map(int, input().split()))
    n, a, b, t1, t2, t3, t4, t5, t6 = map(int, input().split())
    if a == b:
        print(0)
        continue
    
    ans = solve((b - a) % n, t1, t2, t3, t4, t5, t6)
    #print("!!", ans)
    t1, t3 = t3, t1
    t2, t4 = t4, t2
    t5, t6 = t6, t5
    ans = min(ans, solve((a - b) % n, t1, t2, t3, t4, t5, t6))
    print(ans)
