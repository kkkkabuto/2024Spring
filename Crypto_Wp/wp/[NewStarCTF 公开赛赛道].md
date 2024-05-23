# Crypto
## 1.unusual_base
- 题目：
```python
from secret import flag
from Crypto.Util.number import *
from random import shuffle
from string import ascii_lowercase, ascii_uppercase, digits

alphabet = ascii_uppercase + ascii_lowercase + digits + "$&"
alphabet = list(alphabet)
bits = ""
pad_len = len(flag) % 3

for f in flag:
    bits += bin(f)[2:].rjust(8, "0")
bits += "0000" * pad_len
encoded = ""
shuffle(alphabet)
alphabet = "".join(alphabet)
for i in range(0, len(bits), 6):
    encoded += alphabet[int(bits[i : i + 6], 2)]
encoded += "%" * pad_len
print(f'encoded = "{encoded}"')
print(f'alphabet = "{alphabet}"')
# alphabet = "c5PKAQmgI&qSdyDZYCbOV2seXGloLwtFW3f9n7j481UMHBp6vNETRJa$rxuz0hik"
# encoded = "GjN3G$B3de58ym&7wQh9dgVNGQhfG2hndsGjlOyEdaxRFY%"

```

- EXP：
```python
# -*- coding: utf-8 -*-
# 一定要添加编码声明，不然会报错
from Crypto.Util.number import *

alphabet = "c5PKAQmgI&qSdyDZYCbOV2seXGloLwtFW3f9n7j481UMHBp6vNETRJa$rxuz0hik"
encoded = "GjN3G$B3de58ym&7wQh9dgVNGQhfG2hndsGjlOyEdaxRFY%"

binary_indices = ""
for char in encoded:
    if char in alphabet:
        index = alphabet.index(char)
        binary_index = bin(index)[2:].zfill(
            6
        )  # Convert index to binary and pad with zeros
        binary_indices += binary_index
    else:
        print(f"Character '{char}' not found in 'alphabet'.")

print("Binary representation of indices:", binary_indices)
# 长度是276
print(len(binary_indices))
# 去掉末尾的四个0
enc = int(binary_indices, 2)
enc >>= 4
# print(bin(enc))
flag = ""
for i in range(enc.bit_length() // 8 + 1):
    a = enc % 256
    flag += chr(a)
    enc >>= 8  
print(flag[::-1])
# flag{a1ph4bet_c0u1d_be_d1ffi3r3nt}

```


## 2.Affine
```python
from secret import flag
from Crypto.Util.number import *

a = getPrime(8)
b = getPrime(8)

ciphertext = []

for f in flag:
    ciphertext.append((a * f + b) % 0x100)

print(bytes(ciphertext))
# b"\xb1\x83\x82T\x10\x80\xc9O\x84\xc9<\x0f\xf2\x82\x9a\xc9\x9b8'\x9b<\xdb\x9b\x9b\x82\xc8\xe0V"
```

- EXP:
注意编码，python3不注明会报错
```python
# -*- coding: utf-8 -*-
from Crypto.Util.number import *

enc = b"\xb1\x83\x82T\x10\x80\xc9O\x84\xc9<\x0f\xf2\x82\x9a\xc9\x9b8'\x9b<\xdb\x9b\x9b\x82\xc8\xe0V"

# 先找到8位的素数
list_a = []
for a in range(128, 257):
    if isPrime(a):
        list_a.append(a)
print(list_a)

# 爆破a，b
flag = b"flag{"
for a in list_a:  # 爆破a和b
    for b in list_a:
        s = 0
        for i in range(5):
            if (a * flag[i] + b) % 0x100 == enc[i]:
                s += 1
        if s == 5:
            print(a, b)
            # 163 191

# 解密
flag = ""
a = 163
b = 191
re_a = inverse(a, 0x100)
for i in enc:
    f = (i - b) * re_a % 0x100
    flag += chr(f)
print(flag)
# flag{Kn0wn_p1aint3xt_4ttack}

```

## 3.robot
```python
from hashlib import sha256
from secret import flag
from base64 import *
import random

cipher = []

def fun1(x):
    return sha256(x.encode()).hexdigest()

def fun2(x):
    return pow(114514,ord(x),1919810)

def fun3(x):
    key = random.randint(0,1145141919810)
    ans = x.encode()
    if key & 1:
        ans = b32encode(ans)
    key >>= 1
    if key & 1:
        ans = b64encode(ans)
    key >>= 1
    if key & 1:
        ans = b16encode(ans)
    key >>= 1
    return ans

def encrypt(msg):
    res = []
    for i in msg:
        tmp = list(map(random.choice([fun1,fun2,fun3]),[i]))[0]
        res.append(tmp)
    return res

print(encrypt(flag))

'''
['252f10c83610ebca1a059c0bae8255eba2f95be4d1d7bcfa89d7248a82d9f111', 1495846, 1452754, b'M4======', '021fb596db81e6d02bf3d2586ee3981fe519f275c0ac9ca76bbcf2ebb4097d96', '2e7d2c03a9507ae265ecf5b5356885a53393a2029d241394997265a1a25aefc6', '4b227777d4dd1fc61c6f884f48641d02b4d121d3fd328cb08b5531fcacdabf8a', b'Tg==', '1b16b1df538ba12dc3f97edbb85caa7050d46c148134290feba80f8236c83db9', b'52304539505430395054303D', 'e3b98a4da31a127d4bde6e43033f66ba274cab0eb7eb1c70ec41402bf6273dd8', b'58773D3D', '3f39d5c348e5b79d06e842c114e6cc571583bbf44e4b0ebfda1a01ec05745d43', '4e07408562bedb8b60ce05c1decfe3ad16b72230967de01f640b7e4729b49fce', '2e7d2c03a9507ae265ecf5b5356885a53393a2029d241394997265a1a25aefc6', b'T0k9PT09PT0=', '18f5384d58bcb1bba0bcd9e6a6781d1a6ac2cc280c330ecbab6cb7931b721552', b'T0E9PT09PT0=', 825026, 'd2e2adf7177b7a8afddbc12d1634cf23ea1a71020f6a1308070a16400fb68fde', 1455816, b'4F553D3D3D3D3D3D', 1165366, 1242964, b'4F493D3D3D3D3D3D', 652094, 597296, '4e07408562bedb8b60ce05c1decfe3ad16b72230967de01f640b7e4729b49fce', '4b227777d4dd1fc61c6f884f48641d02b4d121d3fd328cb08b5531fcacdabf8a', b'54314539505430395054303D', 1242964, 368664, b'TVU9PT09PT0=', b'cw==', 1602214]
'''
```
- EXP:
```python
from hashlib import sha256
import base64
import string
import re


table = string.printable


def fun1(x):
    for t in table:
        if sha256(t.encode()).hexdigest() == x:
            return t


def fun2(x):
    for t in table:
        if pow(114514, ord(t), 1919810) == x:
            return t


def fun3(s):
    base16_dic = r"^[A-F0-9=]*$"
    base32_dic = r"^[A-Z2-7=]*$"
    base64_dic = r"^[A-Za-z0-9/+=]*$"

    n = 0
    while True:
        n += 1
        t = s.decode()
        try:
            if t != "":
                if re.match(base16_dic, t):
                    s = base64.b16decode(s)
                    print(str(s) + " base16")
                elif re.match(base32_dic, t):
                    s = base64.b32decode(s)
                    print(str(s) + " base32")
                elif re.match(base64_dic, t):
                    s = base64.b64decode(s)
                    print(str(s) + " base64")
        except:
            return t


cipher = [
    "252f10c83610ebca1a059c0bae8255eba2f95be4d1d7bcfa89d7248a82d9f111",
    1495846,
    1452754,
    b"M4======",
    "021fb596db81e6d02bf3d2586ee3981fe519f275c0ac9ca76bbcf2ebb4097d96",
    "2e7d2c03a9507ae265ecf5b5356885a53393a2029d241394997265a1a25aefc6",
    "4b227777d4dd1fc61c6f884f48641d02b4d121d3fd328cb08b5531fcacdabf8a",
    b"Tg==",
    "1b16b1df538ba12dc3f97edbb85caa7050d46c148134290feba80f8236c83db9",
    b"52304539505430395054303D",
    "e3b98a4da31a127d4bde6e43033f66ba274cab0eb7eb1c70ec41402bf6273dd8",
    b"58773D3D",
    "3f39d5c348e5b79d06e842c114e6cc571583bbf44e4b0ebfda1a01ec05745d43",
    "4e07408562bedb8b60ce05c1decfe3ad16b72230967de01f640b7e4729b49fce",
    "2e7d2c03a9507ae265ecf5b5356885a53393a2029d241394997265a1a25aefc6",
    b"T0k9PT09PT0=",
    "18f5384d58bcb1bba0bcd9e6a6781d1a6ac2cc280c330ecbab6cb7931b721552",
    b"T0E9PT09PT0=",
    825026,
    "d2e2adf7177b7a8afddbc12d1634cf23ea1a71020f6a1308070a16400fb68fde",
    1455816,
    b"4F553D3D3D3D3D3D",
    1165366,
    1242964,
    b"4F493D3D3D3D3D3D",
    652094,
    597296,
    "4e07408562bedb8b60ce05c1decfe3ad16b72230967de01f640b7e4729b49fce",
    "4b227777d4dd1fc61c6f884f48641d02b4d121d3fd328cb08b5531fcacdabf8a",
    b"54314539505430395054303D",
    1242964,
    368664,
    b"TVU9PT09PT0=",
    b"cw==",
    1602214,
]


flag = ""
for i in cipher:
    if type(i) == str:
        flag += fun1(i)
    elif type(i) == int:
        flag += fun2(i)
    elif type(i) == bytes:
        flag += fun3(i)
print(flag)
# flag{c4Nn0t_D3crYpt_buT_r3p34t_Yes}

```


# Reverse
## 1.前可见古人，后得见来者
- EXP:
```python
# 先异或解密
cipher = b"Q[LVYMPVTC}LCS}PCS}GP}LCS}N@J_"
flag = ""
for i in cipher:
    flag += chr(i ^ 0x22)
print(flag)
# synt{ortva_naq_raq_er_naq_lbh}

```

```python
# 凯撒爆破
def caesar_cipher_decrypt(ciphertext, shift):
    plaintext = ""
    for char in ciphertext:
        if char.isalpha():
            shifted = ord(char) - shift
            if char.islower():
                if shifted < ord('a'):
                    shifted += 26
            elif char.isupper():
                if shifted < ord('A'):
                    shifted += 26
            plaintext += chr(shifted)
        else:
            plaintext += char
    return plaintext


def caesar_cipher_brute_force(ciphertext):
    for shift in range(26):
        plaintext = caesar_cipher_decrypt(ciphertext, shift)
        print("Shift:", shift, "Plaintext:", plaintext)


cipher = "synt{ortva_naq_raq_er_naq_lbh}"
caesar_cipher_brute_force(cipher)

# flag{begin_and_end_re_and_you}
```