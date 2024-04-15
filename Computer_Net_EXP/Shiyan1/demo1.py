from Cryptodome.Cipher import AES
import binascii
import itertools

msg = b"0000000000000000"
cip = b"4c2165086ee1e0fbfbc63029b63466e1"

# 生成所有可能的年、月、日组合
years = range(1900, 2100)
months = range(1, 13)
days = range(1, 32)

# 使用 itertools.product 生成所有可能的生日组合
birthdays = itertools.product(years, months, days)

# 遍历所有生日组合
for birthday in birthdays:
    # 将生日转换为字符串格式的密钥
    key = f"{birthday[0]:04d}{birthday[1]:02d}{birthday[2]:02d}00000000"
    key_bytes = key.encode("utf-8")

    # 使用 AES ECB 模式加密
    aes = AES.new(key_bytes, AES.MODE_ECB)
    encrypted_text = aes.encrypt(msg)
    cipher = binascii.b2a_hex(encrypted_text)

    # 如果加密后的密文与目标密文匹配，则打印密钥并退出循环
    if cip == cipher:
        print("cipher: ", cip)
        print("Key Found:", key)
        break
else:
    print("Key not found")
