from Crypto.Cipher import DES
import os
import time

start_time = time.time()

# 生成随机的8个字节作为密钥
key = os.urandom(8)

# 使用DES加密文件
with open("file_GB.txt", "rb") as f:
    file_data = f.read()

# 分块加密，并将加密后的数据保存到encrypted_file_GB_DES.txt中
with open("encrypted_file_GB_DES.txt", "wb") as f:
    chunk_size = 8  # DES block size is 64 bits (8 bytes)
    offset = 0
    while offset < len(file_data):
        chunk = file_data[offset : offset + chunk_size]
        cipher_des = DES.new(key, DES.MODE_ECB)
        encrypted_chunk = cipher_des.encrypt(chunk)
        f.write(encrypted_chunk)
        offset += chunk_size


end_time = time.time()
execution_time = end_time - start_time
print(f"DES加密1GB文件时间: {execution_time} 秒\n")
