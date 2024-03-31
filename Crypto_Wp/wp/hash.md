# hash函数
- [DASCTF NOV X联合出题人2022年度积分榜争夺赛！]easy_hash
```python
from Crypto.Util.number import bytes_to_long, long_to_bytes
from zlib import crc32
from secret import *
P = 93327214260434303138080906179883696131283277062733597039773430143631378719403851851296505697016458801222349445773245718371527858795457860775687842513513120173676986599209741174960099561600915819416543039173509037555167973076303047419790245327596338909743308199889740594091849756693219926218111062780849456373
def myhash(x):
    res = []
    end = b""
    bytescipher = long_to_bytes(x)
    a = bytescipher[:len(bytescipher) % 8]
    res.append(a)
    res.append(long_to_bytes(crc32(a)))
    t = (len(bytescipher) // 8)
    bytescipher = bytescipher[len(bytescipher) % 8:]
    for i in range(t):
        a = bytescipher[i*8:i*8+8]
        res.append(a)
        res.append(long_to_bytes(crc32(a)))
    for i in res:
        end += i
    res = bytes_to_long(end)
    res = (res + (res >> 500)) & 2**(500)-1
    return res
def encode(pt):
    a=[]
    b=[]
    a.append(myhash(pt))
    for i in range(3):
        a.append(myhash(a[i]))
    for j in range(4):
        secret=(a[0] + a[1] * a[j] + a[2] * a[j] ** 2 + a[3] * a[j] ** 3) % P
        b.append([a[j],secret])
    return b
pt = bytes_to_long(flag.encode())
FLAG=encode(pt)
print(FLAG[1])
#[1768672211043417187765307394749760760531160781992300779145800061219666992833602547480090118225665457075744297987672863699370162614965380859290914620736, 89139545215288033432210221492974990584987914397112840989583439688211128705545477536596587262069032020212762581490561288493533363888589066045095054475929099275247145877699370608950340925139625068446642116123285918461312297390611577025368805438078034230342490499137494400676347225155752865648820807846513044723]

```

- myhash()返回一个哈希值
- encode()进行加密，给了FLAG[1]，也就是a[1]和由a[i]计算得到的secret[1]
```python
for i in range(3):
    a.append(myhash(a[i]))
```
- 观察这个函数，发现得到a[1]就可以用myhash()计算a[2]和a[3]，之后使用secret[1]就可以得到a[0]
- a[0]转字节后就包含flag，只是中间多了一些混淆字节，因为myhash()就是增加一些crc冗余位，第一眼没看出来（麻了）
```python
from Crypto.Util.number import *
from zlib import crc32


def myhash(x):
    res = []
    end = b""
    bytescipher = long_to_bytes(x)
    a = bytescipher[: len(bytescipher) % 8]
    res.append(a)
    res.append(long_to_bytes(crc32(a)))
    t = len(bytescipher) // 8
    bytescipher = bytescipher[len(bytescipher) % 8 :]
    for i in range(t):
        a = bytescipher[i * 8 : i * 8 + 8]
        res.append(a)
        res.append(long_to_bytes(crc32(a)))
    for i in res:
        end += i
    res = bytes_to_long(end)
    res = (res + (res >> 500)) & 2 ** (500) - 1
    return res


P = 93327214260434303138080906179883696131283277062733597039773430143631378719403851851296505697016458801222349445773245718371527858795457860775687842513513120173676986599209741174960099561600915819416543039173509037555167973076303047419790245327596338909743308199889740594091849756693219926218111062780849456373
a1, secret1 = [
    1768672211043417187765307394749760760531160781992300779145800061219666992833602547480090118225665457075744297987672863699370162614965380859290914620736,
    89139545215288033432210221492974990584987914397112840989583439688211128705545477536596587262069032020212762581490561288493533363888589066045095054475929099275247145877699370608950340925139625068446642116123285918461312297390611577025368805438078034230342490499137494400676347225155752865648820807846513044723,
]
print(a1.bit_length())
a2 = myhash(a1)
a3 = myhash(a2)
a0 = (secret1 - a1 * a1 - a2 * a1**2 - a3 * a1**3) % P
flag = long_to_bytes(a0)
print(flag)

import string

table = string.ascii_letters.encode() + b"_{}" + string.digits.encode()
print(table)
for i in flag:
    if i in table:
        print(chr(i), end="")
# b'DA\xd4\x17\xe9\xf8SCTF{th1\x98\xf8\xa5$s_is_theS\x83\xbf\xc9_fe3st_q\x8f\xa9\xd4\xacuest1on}\x07.B\xce'
# b'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_{}0123456789'
# DASCTF{th1s_is_theS_fe3st_quest1on}B
# 还要去掉S和B DASCTF{th1s_is_the_fe3st_quest1on}

```