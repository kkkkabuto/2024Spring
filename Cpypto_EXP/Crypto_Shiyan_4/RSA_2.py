# from Crypto.PublicKey import RSA
# from Crypto.Cipher import PKCS1_OAEP
# import time

# # 记录时间
# start_time = time.time()

# # 生成RSA密钥对
# key = RSA.generate(2048)

# # 公钥和私钥
# public_key = key.publickey()
# private_key = key

# # 保存密钥对到文件
# with open("public_key_GB.pem", "wb") as f:
#     f.write(public_key.export_key())

# with open("private_key_GB.pem", "wb") as f:
#     f.write(private_key.export_key())

# # 使用公钥加密文件
# cipher_rsa = PKCS1_OAEP.new(public_key)

# with open("file_GB.txt", "rb") as f:
#     file_data = f.read()

# # 分块加密，并将加密后的数据保存到encrypted_file_MB_RSA.txt中
# with open("encrypted_file_GB_RSA.txt", "wb") as f:
#     chunk_size = 190  # 2048 / 8 - 11
#     offset = 0
#     while offset < len(file_data):
#         chunk = file_data[offset : offset + chunk_size]
#         encrypted_chunk = cipher_rsa.encrypt(chunk)
#         f.write(encrypted_chunk)
#         offset += chunk_size


# # 使用私钥解密文件
# cipher_rsa = PKCS1_OAEP.new(private_key)

# with open("encrypted_file_GB_RSA.txt", "rb") as f:
#     encrypted_data = f.read()

# # 解密文件，并将解密后的数据保存到decrypted_file.txt中
# with open("decrypted_file_GB_RSA.txt", "wb") as f:
#     offset = 0
#     while offset < len(encrypted_data):
#         chunk = encrypted_data[offset : offset + 256]  # 2048 / 8
#         decrypted_chunk = cipher_rsa.decrypt(chunk)
#         f.write(decrypted_chunk)
#         offset += 256

# end_time = time.time()
# execution_time = end_time - start_time
# print(f"RSA加密1GB文件时间: {execution_time} 秒\n")


from Crypto.PublicKey import RSA
from Crypto.Cipher import PKCS1_OAEP
from concurrent.futures import ThreadPoolExecutor
import time


def encrypt_chunk(chunk):
    cipher_rsa = PKCS1_OAEP.new(public_key)
    return cipher_rsa.encrypt(chunk)


def decrypt_chunk(chunk):
    cipher_rsa = PKCS1_OAEP.new(private_key)
    return cipher_rsa.decrypt(chunk)


start_time = time.time()

# 生成RSA密钥对
key = RSA.generate(2048)

# 公钥和私钥
public_key = key.publickey()
private_key = key

# 保存密钥对到文件
with open("public_key_GB.pem", "wb") as f:
    f.write(public_key.export_key())

with open("private_key_GB.pem", "wb") as f:
    f.write(private_key.export_key())

# 使用公钥加密文件
with open("file_GB.txt", "rb") as f:
    file_data = f.read()

# 分块加密，并将加密后的数据保存到encrypted_file_GB_RSA.txt中
with open("encrypted_file_GB_RSA.txt", "wb") as f:
    chunk_size = 190  # 2048 / 8 - 11
    offset = 0
    with ThreadPoolExecutor(max_workers=64) as executor:
        while offset < len(file_data):
            chunk = file_data[offset : offset + chunk_size]
            encrypted_chunk = executor.submit(encrypt_chunk, chunk)
            f.write(encrypted_chunk.result())
            offset += chunk_size

end_time = time.time()
execution_time = end_time - start_time
print(f"RSA加密1GB文件时间: {execution_time} 秒")
