# 异或 + base64恢复key
- 题目wctf Crypto: yORs Truly <3 
```python
import base64

plaintext = "A string of text can be encrypted by applying the bitwise XOR operator to every character using a given key"
key = ""  # I have lost the key!


def byte_xor(ba1, ba2):
    return bytes([_a ^ _b for _a, _b in zip(ba1, ba2)])


ciphertext_b64 = base64.b64encode(byte_xor(key.encode(), plaintext.encode()))
ciphertext_decoded = base64.b64decode("NkMHEgkxXjV/BlN/ElUKMVZQEzFtGzpsVTgGDw==")
print(ciphertext_decoded)
# b"6C\x07\x12\t1^5\x7f\x06S\x7f\x12U\n1VP\x131m\x1b:lU8\x06\x0f"
```

- 脚本：
```python 
import base64

plaintext = "A string of text can be encrypted by applying the bitwise XOR operator to every character using a given key"


def byte_xor(ba1, ba2):
    return bytes([_a ^ _b for _a, _b in zip(ba1, ba2)])


c = b"6C\x07\x12\t1^5\x7f\x06S\x7f\x12U\n1VP\x131m\x1b:lU8\x06\x0f"
m = plaintext
k = byte_xor(c, m.encode())
print(k)

# b'wctf{X0R_i5_f0rEv3r_My_L0Ve}'

```