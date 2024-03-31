import secrets
from sympy import gcd, nextprime
import time

# 记录时间
start_time = time.time()

# 选取p,q都是2048位的素数
p = secrets.randbits(2048)
p = nextprime(p)
q = secrets.randbits(2048)
q = nextprime(q)

n = p * q
phi = (p - 1) * (q - 1)

# 随机选择明文,1024位
m = secrets.randbits(1024)
with open("out.txt", "w") as f:
    f.write(f"p = {p}\n")
    f.write(f"q = {q}\n")
    f.write(f"n = {n}\n")
    f.write(f"m = {m}\n")

# 随机选择e
while True:
    # 512位
    e = secrets.randbits(512)
    if gcd(e, phi) == 1:
        with open("out.txt", "a") as f:
            f.write(f"e = {e}\n")
        break

c = pow(m, e, n)
with open("out.txt", "a") as f:
    f.write(f"c = {c}\n")

end_time = time.time()
execution_time = end_time - start_time
with open("out.txt", "a") as f:
    f.write(f"程序运行时间: {execution_time} 秒\n")
