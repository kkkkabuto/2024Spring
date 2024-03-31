- LFSR介绍 https://www.anquanke.com/post/id/181811#h2-0
# 例题 [CISCN2018]oldstreamgame1
```python
flag = "flag{xxxxxxxxxxxxxxxx}"
assert flag.startswith("flag{")
assert flag.endswith("}")
assert len(flag)==14

def lfsr(R,mask):
    output = (R << 1) & 0xffffffff
    i=(R&mask)&0xffffffff
    lastbit=0
    while i!=0:
        lastbit^=(i&1)
        i=i>>1
    output^=lastbit
    return (output,lastbit)

R=int(flag[5:-1],16)
mask = 0b10100100000010000000100010010100

f=open("key","w")
for i in range(100):
    tmp=0
    for j in range(8):
        (R,out)=lfsr(R,mask)
        tmp=(tmp << 1)^out
    f.write(chr(tmp))
f.close()
```

# 类似LFSR
- [Zer0pts2020]ROR
```python
import random
from secret import flag

ror = lambda x, l, b: (x >> l) | ((x & ((1 << l) - 1)) << (b - l))

N = 1
for base in [2, 3, 7]:
    N *= pow(base, random.randint(123, 456))
e = random.randint(271828, 314159)

m = int.from_bytes(flag, byteorder="big")
assert m.bit_length() < N.bit_length()

for i in range(m.bit_length()):
    print(pow(ror(m, i, m.bit_length()), e, N))
```
- ror表示循环右移操作，接收三个参数：x表示需要循环右移的操作数，l表示循环右移的位数，b表示x的位数
- N的计算表明N是一个偶数，e的值无关紧要
```python
for i in range(m.bit_length()):
    print(pow(ror(m, i, m.bit_length()), e, N))
```
- 对m进行循环右移操作，直到最高位移动到最低位
- 只需要关注ror(m, i, m.bit_length())的最低位，如果是偶数(最低位是0)，那么c=pow(ror(m, i, m.bit_length()), e, N)的最低位也是0；如果是奇数，那么c的最低位是1；这样就可以得到m的每一位（m是2进制数）
- 注意第一个c得到的是m的最低位，最后一个c得到m的最高位，最后需要逆序输出
```python
from Crypto.Util.number import long_to_bytes

with open("C:\\Users\\19753\\Desktop\\chall.txt", "r") as f:
    c = f.readlines()

m = ""
for i in c:
    # 读出的是字符串
    i = i.strip()
    # 转为整数处理
    i = int(i, 10)
    if i % 2 == 1:
        m += "1"
    else:
        m += "0"

# m现在是01字符串，先逆序
m = m[::-1]
# 再将m解释为10进制的整数
m = int(m, 2)
print(long_to_bytes(m))
# b'zer0pts{0h_1t_l34ks_th3_l34st_s1gn1f1c4nt_b1t}'

```