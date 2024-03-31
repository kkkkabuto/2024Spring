# 题目
- [ACTF新生赛2020]crypto-aes

```python
from Cryptodome.Cipher import AES
import os
import gmpy2
from flag import FLAG
from Cryptodome.Util.number import *


def main():
    key = os.urandom(2) * 16
    iv = os.urandom(16)
    print(bytes_to_long(key) ^ bytes_to_long(iv))
    aes = AES.new(key, AES.MODE_CBC, iv)
    enc_flag = aes.encrypt(FLAG)
    print(enc_flag)


if __name__ == "__main__":
    main()

# 91144196586662942563895769614300232343026691029427747065707381728622849079757
# b'\x8c-\xcd\xde\xa7\xe9\x7f.b\x8aKs\xf1\xba\xc75\xc4d\x13\x07\xac\xa4&\xd6\x91\xfe\xf3\x14\x10|\xf8p'

```

# 解题
- 关键是求key和iv
首先看到key是32bytes，iv是16bytes，两者异或的结果（xor）是32bytes，那么xor的高16bytes就是key的高16bytes
xor的低16bytes是异或的结果，而key的高16bytes和低16bytes相同，那么key就得到了
```
key = xor[16:] * 2
```
xor的低16bytes只要再和key低16bytes异或就得到iv
```
iv = key[16:] ^ xor[16:]
```

# 脚本
```python
from Crypto.Cipher import AES
import os
import gmpy2
from Crypto.Util.number import *

xor = 91144196586662942563895769614300232343026691029427747065707381728622849079757
enc_flag = b"\x8c-\xcd\xde\xa7\xe9\x7f.b\x8aKs\xf1\xba\xc75\xc4d\x13\x07\xac\xa4&\xd6\x91\xfe\xf3\x14\x10|\xf8p"
out = long_to_bytes(xor)
key = out[:16] * 2
print(key)
iv = bytes_to_long(key[16:]) ^ bytes_to_long(out[16:])
iv = long_to_bytes(iv)
aes = AES.new(key, AES.MODE_CBC, iv)
flag = aes.decrypt(enc_flag)
print(flag)
```

# 代码学习
- urandom
语法 os.urandom(size)
参数：
&ensp;&ensp;&ensp;&ensp;size:字符串随机字节大小（有几个字节）
&ensp;&ensp;&ensp;&ensp;返回值：一个字符串

- AES.new(key, mode, *args, **kwargs)
param key(参数密钥):
在对称密码中使用的秘密密钥。
它必须为16、24或32个字节长（分别用于AES-128， AES-192或AES-256）。
mode（模式）：
模式（支持的MODE_*常量之一）–用于加密或解密的链接模式。
学习链接：https://pycryptodome.readthedocs.io/en/latest/src/cipher/aes.html
Keyword Arguments（关键字参数）：
IV（字节，字节组，memoryview） - （只适用于MODE_CBC，MODE_CFB，MODE_OFB，和MODE_OPENPGP模式）。
用于加密或解密的初始化向量。
对于MODE_CBC，MODE_CFB和MODE_OFB它必须是16个字节。

# coding
- [CISCN2018]sm
```python
from Crypto.Util.number import getPrime,long_to_bytes,bytes_to_long
from Crypto.Cipher import AES
import hashlib
from random import randint
def gen512num():
    order=[]
    while len(order)!=512:
        tmp=randint(1,512)
        if tmp not in order:
            order.append(tmp)
    ps=[]
    for i in range(512):
        p=getPrime(512-order[i]+10)
        pre=bin(p)[2:][0:(512-order[i])]+"1"
        ps.append(int(pre+"0"*(512-len(pre)),2))
    return ps

def run():
    choose=getPrime(512)
    ps=gen512num()
    print "gen over"
    bchoose=bin(choose)[2:]
    r=0
    bchoose = "0"*(512-len(bchoose))+bchoose
    for i in range(512):
        if bchoose[i]=='1':
            r=r^ps[i]
    flag=open("flag","r").read()

    key=long_to_bytes(int(hashlib.md5(long_to_bytes(choose)).hexdigest(),16))
    aes_obj = AES.new(key, AES.MODE_ECB)
    ef=aes_obj.encrypt(flag).encode("base64")

    open("r", "w").write(str(r))
    open("ef","w").write(ef)
    gg=""
    for p in ps:
        gg+=str(p)+"\n"
    open("ps","w").write(gg)

run()
```

- 分析题目，与flag有关的只有choose，拿到choose之后aes解密就能得到flag
- choose是512位随机数，看choose与其他变量的关系，bchoose是choose的二进制填充至512位后的值
- 对于bchoose中的每一位，判断是否位1，让r与ps[i]异或
- r的值异或后会被刷新，但是只要再异或一次r就能得到ps[i]，关键是bchoose到底哪些位是1
- 爆破的画肯定是不行的，有2^512种可能，只能看看ps是如何生成的了
- 分析gen512num()函数：主要操作是生成512个相似结构的512位数字，每个数都由一个长度不定的前缀和随后的0填充组成。对异或来讲，只有这个前缀是有意义的。虽然是通过随机数生成的，但事实上这个前缀的长度是可以反算出来的，脚本如下：
```python
f = open("C:\\Users\\19753\\Desktop\\ps", "r")
line = f.readlines()
s = []
for i in range(len(line)):
    line[i] = int(line[i].strip("\n"))
    t = line[i]
    len = 0
    while t % 2 == 0:
        len += 1
        t = t >> 1
    s.append(512 - len)
# s.sort()  # 一旦排序了就看出其中玄机了
print(s)
```
- 不难发现s正好是1-512的无重复数组，与
```python
while len(order)!=512:
        tmp=randint(1,512)
        if tmp not in order:
            order.append(tmp)
```
吻合，但是注意，我们可以排序了之后观察，但是不能在后面使用排序过后的s，因为解出choose是要知道ps中哪几行的数被异或过，那么这些数原来的顺序也是必要的信息，不能擅自修改。
- 至此，前缀长度是1-512的不重复数组这个特殊结构，让我们反推choose的算法难度大幅降低（但是这很不现实，随机生成的长度这么巧的也太难了）。
这是因为比如最后一个值为1的位，导致它值为1的可能，只有前缀长为它对应位置的那个数（因为前缀的结尾必为1）。
异或这个数把这个数造成的影响消除，这样从后往前找可以再发现一个位仅受ps中的一个数的影响的情况，依次类推，就可以还原出 r 到底异或了哪些数，找到都是异或了哪些数，根据 r 的产生的方式，我们就能知道choose的哪些位是1。
- 脚本如下：
```python
f1 = open("C:\\Users\\19753\\Desktop\\r", "r")

k = int(f1.read())
m = list("0" * 512)  # bchoose

for i in range(511, -1, -1):
    if (k >> (511 - i)) % 2 == 1:
        k = k ^ line[s.index(i + 1)]
        m[s.index(i + 1)] = "1"
    else:
        m[s.index(i + 1)] = "0"
choose = int("0b" + "".join(m), 2)
print(choose)
# choose=11400599473028310480620591074112690318799501642425666449519888152497765475409346201248744734864375690112798434541063767944755958258558428437088372812844657

```
- 最后aes解密就行：
```python
key = long_to_bytes(int(hashlib.md5(long_to_bytes(choose)).hexdigest(), 16))
aes_obj = AES.new(key, AES.MODE_ECB)
f2 = open("C:\\Users\\19753\\Desktop\\ef", "rb")
ef = f2.read()
ef = base64.b64decode(ef)
result = aes_obj.decrypt(ef)
print(result)
# flag{shemir_alotof_in_wctf_fun!}
```