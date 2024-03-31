from Crypto.Cipher import DES
from Crypto.Random import get_random_bytes


# 加密16进制数，观察改变明文一位后密文的变化
# 加密函数
def encrypt(input_hex, key):
    des = DES.new(key, DES.MODE_ECB)
    input_bytes = bytes.fromhex(input_hex)
    encrypted_bytes = des.encrypt(input_bytes)
    return encrypted_bytes.hex()


# 使用生成的随机密钥
key = get_random_bytes(8)

# 输入十六进制数，改变最后一位
input_hex = "0123456789ABCDEF"
input_change = "0123456789ABCDEE"

# 加密原始输入
encrypted_original = encrypt(input_hex, key)

# 比较改变一个位后的输出
encrypted_change = encrypt(input_change, key)

print(f"改变前：{encrypted_original}")
print(f"改变后：{encrypted_change}")
