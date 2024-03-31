# 这道题还挺简单的
- [NewStarCTF 2023 公开赛道]babyxor
```python
from secret import *

ciphertext = []

for f in flag:
    ciphertext.append(f ^ key)

print(bytes(ciphertext).hex())
e9e3eee8f4f7bffdd0bebad0fcf6e2e2bcfbfdf6d0eee1ebd0eabbf5f6aeaeaeaeaeaef2
```
- 很简单，flag中每个字符和key异或得到ciphertext
- 首先将ciphertext还原成字节串ciphertext = bytes.fromhex(ciphertext)
- 因为是一个字符一个字符地异或，一个字符占一个字节，所以key的范围就是一个字节大小0xEE
- 最后检测得到的flag是否以“flag”“NSCTF”“nsctf”开头
- 注意flag_str = bytes(flag).decode("latin-1")，不能使用"utf-8"编码，会报错
- EXP：
```python
ciphertext = "e9e3eee8f4f7bffdd0bebad0fcf6e2e2bcfbfdf6d0eee1ebd0eabbf5f6aeaeaeaeaeaef2"
ciphertext = bytes.fromhex(ciphertext)
flag = []
for key in range(0xEE):
    flag.clear()  # 每次迭代前清空flag列表
    for c in ciphertext:
        flag.append(c ^ key)
    # 判断flag的开头是否是指定字符串
    flag_str = bytes(flag).decode("latin-1")  # 将字节序列转换为字符串
    if (
        flag_str.startswith("flag")
        or flag_str.startswith("nsctf")
        or flag_str.startswith("NSCTF")
    ):
        print(flag_str)
# flag{x0r_15_symm3try_and_e4zy!!!!!!}
```

- 再来看一道[NewStarCTF 公开赛赛道]eazyxor
```python
from os import urandom
from secret import flag
key = urandom(1)

def xor(plaintext, key):
    ret = []
    for i in range(len(plaintext)):
        ret.append(plaintext[i] ^ key[0])
    return bytes(ret)

ciphertext = xor(flag, key)

print(ciphertext.hex())
```

- EXP:
```python
c = "9b919c9a8685cd8fa294c8a28c88cc89cea2ce9c878480"


def xor(plaintext, key):
    ret = []
    for i in range(len(plaintext)):
        ret.append(plaintext[i] ^ key)
    return bytes(ret)


for i in range(1, 255):
    s = bytes.fromhex(c)
    flag = xor(s, i)
    flag_str = bytes(flag).decode("latin-1")  # 将字节序列转换为字符串
    if (
        flag_str.startswith("flag")
        or flag_str.startswith("nsctf")
        or flag_str.startswith("NSCTF")
    ):
        print(flag_str, i)
        break

```