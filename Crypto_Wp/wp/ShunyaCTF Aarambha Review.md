# Cipher Conundrum
- 题目
```python
import base64
from string import punctuation

alphabet = list(punctuation)
data = "0CTF{}"


def main():
    key = ""
    encrypted = "".join([chr(ord(x) ^ int(key, 16)) for x in data])
    encrypted_data_base64 = base64.b64encode(encrypted.encode()).decode()

    print(encrypted_data_base64)


main()
# encrypted_data_base64 = "1JjVq9W81a7Vk9Sd1YfVhdWN1J/VgdWF1JvVm9W31YHUn9W31YbVjdSb1YzUndW31ZzUmNW31YrUm9W31YvUmNWF1ZjVhNSb1ZDVlQ=="

```
- 注意到key是数字，也就是说key只有一个，只需要将data的第一位与encrypted的第一位异或就得到key，同样第二位与第二位异或也能得到key
- 注意encrypted_data_base64 = base64.b64encode(encrypted.encode()).decode()，需要使用相反的逆操作还原encrypted
- EXP:
```python
import base64

enc = "1JjVq9W81a7Vk9Sd1YfVhdWN1J/VgdWF1JvVm9W31YHUn9W31YbVjdSb1YzUndW31ZzUmNW31YrUm9W31YvUmNWF1ZjVhNSb1ZDVlQ=="
enc = base64.b64decode(enc.encode()).decode()
print(enc)
data = "0CTF{"
key = ord(data[0]) ^ ord(enc[0])
print(key)
data = ''.join([chr(ord(enc[i]) ^ key) for i in range(len(enc))])
print(data)
# 1320
# 0CTF{5ome7im3s_i7_ne3d5_t0_b3_c0mpl3x}
```

# Echoes of Encryption
- 题目
```python
import random
import string

def encrypt_string(input_string, seed):
    random.seed(seed)
    
    allowed_chars = string.ascii_letters + string.digits
    key = ''.join(random.choices(allowed_chars, k=len(input_string)))
    encrypted_string = ''
    for i in range(len(input_string)):
        encrypted_char = chr(ord(input_string[i]) ^ ord(key[i]))
        encrypted_string += encrypted_char
    return encrypted_string.encode().hex()


seed_value = 
input_string = ""
encrypted = encrypt_string(input_string, seed_value)

# 5e04610a22042638723c571e1a5436142764061f39176b4414204636251072220a35583a60234d2d28082b
```
- Description:
In December 2022, my friend Alok's device was hacked. Upon investigation, he discovered that the breach was due to a vulnerability in the Nvidia SMC which had been recently discovered and published for research purposes on the same day he was hacked.

PS- In the end, only numbers matter to grow a plant from a seed!!

- key是由随机数种子生成，相同的种子必然得到相同的key，根据提示搜索“Nvidia SMC December 2022漏洞”，得到漏洞的命名“CVE-2022-42269”，种子就是202242269，反正肯定在这里面，多试一下
- 输入的input_string需要先转换为字节，再decode
- decrypt_string函数输出不需要转16进制
- EXP:
```python
import random
import string

def decrypt_string(input_string, seed):
    random.seed(seed)

    allowed_chars = string.ascii_letters + string.digits
    key = ''.join(random.choices(allowed_chars, k=len(input_string)))
    decrypted_string = ''
    for i in range(len(input_string)):
        decrypted_char = chr(ord(input_string[i]) ^ ord(key[i]))
        decrypted_string += decrypted_char
    return decrypted_string


seed_value = 202242269
byte_string = "5e04610a22042638723c571e1a5436142764061f39176b4414204636251072220a35583a60234d2d28082b"
byte_string = bytes.fromhex(byte_string)
input_string = byte_string.decode()

encrypted = decrypt_string(input_string, seed_value)
print(encrypted)
# 0CTF{alw4y5_r3ad_7he_d3scr!pti0n_c4r3fully}
```