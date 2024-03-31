from sympy import isprime

p = 3
q = 11
m = 2
n = p * q
phi = (p - 1) * (q - 1)
# 选取e=3
e = 3


# 求逆元
def re(e: int, phi: int) -> int:
    for d in range(1, phi):
        if isprime(d):
            if e * d % phi == 1:
                break
    return d


# 加密
c = pow(m, e, n)
# 解密
d = re(e, phi)
m_decrypted = pow(c, d, n)

print(f"明文:{m}")
print(f"公钥:{n}")
print(f"密文:{c}")
print(f"私钥:{d}")
print(f"解密后明文:{m_decrypted}")
