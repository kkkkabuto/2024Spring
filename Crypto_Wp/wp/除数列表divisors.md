# sagemath中的divisors()
- 题目vctf2024 狂飙
```python
import os
from secrets import flag
from Crypto.Util.number import *
from Crypto.Cipher import AES

m = 88007513702424243702066490849596817304827839547007641526433597788800212065249
key = os.urandom(24)
key = bytes_to_long(key)
n = m % key
flag += (16 - len(flag) % 16) * b"\x00"
iv = os.urandom(16)
aes = AES.new(key, AES.MODE_CBC, iv)
enc_flag = aes.encrypt(flag)

print(n)
print(enc_flag)
print(iv)


# 103560843006078708944833658339172896192389513625588
# b'\xfc\x87\xcb\x8e\x9d\x1a\x17\x86\xd9~\x16)\xbfU\x98D\xfe\x8f\xde\x9c\xb0\xd1\x9e\xe7\xa7\xefiY\x95C\x14\x13C@j1\x9d\x08\xd9\xe7W>F2\x96cm\xeb'
# b'UN\x1d\xe2r<\x1db\x00\xdb\x9a\x84\x1e\x82\xf0\x86'

```

- 意思很简单只要求出key就行，m = k * key + n，但是key有192位，苦于不知道divisors()函数，根本爆破不出来
- divisors()函数返回一个除数列表，里面一定有key
- 注意i = long_to_bytes(i,24)一定要指明24个字节
- sage中运行，虽然会报错，但是能求出flag，感觉这个题出得不太好
```python
from Crypto.Util.number import *
from Crypto.Cipher import AES


m = 88007513702424243702066490849596817304827839547007641526433597788800212065249
n = 103560843006078708944833658339172896192389513625588
iv = b"UN\x1d\xe2r<\x1db\x00\xdb\x9a\x84\x1e\x82\xf0\x86"
enc_flag = b"\xfc\x87\xcb\x8e\x9d\x1a\x17\x86\xd9~\x16)\xbfU\x98D\xfe\x8f\xde\x9c\xb0\xd1\x9e\xe7\xa7\xefiY\x95C\x14\x13C@j1\x9d\x08\xd9\xe7W>F2\x96cm\xeb"
tmp = m - n
for i in tmp.divisors():
    i = long_to_bytes(i,24)
    myaes = AES.new(i, AES.MODE_CBC, iv)
    flag = myaes.decrypt(enc_flag)
    if b'flag{' in flag:
        print(flag)
# b'flag{cf735a4d-f9d9-5110-8a73-5017fc39b1b0}\x00\x00\x00\x00\x00\x00'

```