# 简单异或，循环移位操作
- [NewStarCTF 2023 公开赛道]Rotate Xor
```python
from secret import flag
from os import urandom
from pwn import xor
from Cryptodome.Util.number import *
k1 = getPrime(64)
k2 = getPrime(64)
ROUND = 12
ciphertext = xor(flag, long_to_bytes(k1))
def round_rotate_left(num, step):
    return ((num) << step | num >> (64-step)) & 0xffffffffffffffff
def encrypt_key(key):
    
    for _ in range(ROUND):
        key = round_rotate_left(key, 3) ^ k2
    
    return key
print('ciphertext =', ciphertext)
print('enc_k1 =', encrypt_key(k1))
print('k2 =', k2)

# ciphertext = b'\x8dSyy\xd2\xce\xe2\xd2\x98\x0fth\x9a\xc6\x8e\xbc\xde`zl\xc0\x85\xe0\xe4\xdfQlc'
# enc_k1 = 7318833940520128665
# k2 = 9982833494309156947
```

- 目标是求k1，round_rotate_left()函数是循环左移操作，encrypt_key()函数中对k1循环左移3位再与k2异或，得到的值再循环左移3位后与k2异或，重复12次
- 逆向一下，需要对enc_k1先与k2异或，再循环右移3位，如此重复12轮就得到k1
- 得到k1后与cipher异或，但cipher是字节串呀，不能和k1(数字)异或，发现加密时用的操作是这样：
```python
from pwn import xor
ciphertext = xor(flag, long_to_bytes(k1))
```
- 那么同样装pwn库使用xor函数就可以了
```python
from pwn import xor

ROUND = 12
k2 = 9982833494309156947
enc_k1 = 7318833940520128665
ciphertext = (
    b"\x8dSyy\xd2\xce\xe2\xd2\x98\x0fth\x9a\xc6\x8e\xbc\xde`zl\xc0\x85\xe0\xe4\xdfQlc"
)


def round_rotate_right(num, step):
    return ((num) << (64 - step) | num >> step) & 0xFFFFFFFFFFFFFFFF


def decrypt_key(key):

    for _ in range(ROUND):
        key = round_rotate_right(key ^ k2, 3)
    return key


k1 = decrypt_key(enc_k1)
# print(long_to_bytes(k1))
flag = xor(long_to_bytes(k1), ciphertext)
print(flag)
# b'flag{z3_s0lv3r_15_bri11i4nt}'
```