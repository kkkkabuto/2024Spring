from Crypto.Cipher import DES
from Crypto.Random import get_random_bytes


# 此程序用于对文件进行DES加密和解密操作
# 加密函数
def encrypt_file(input_file, output_file, key):
    des = DES.new(key, DES.MODE_ECB)
    with open(input_file, "rb") as f_in:
        with open(output_file, "wb") as f_out:
            while True:
                chunk = f_in.read(8)
                if len(chunk) == 0:
                    break
                elif len(chunk) % 8 != 0:
                    chunk += b" " * (8 - len(chunk) % 8)  # 填充空格使长度为 8 的倍数
                encrypted_chunk = des.encrypt(chunk)
                f_out.write(encrypted_chunk)


# 解密函数
def decrypt_file(input_file, output_file, key):
    des = DES.new(key, DES.MODE_ECB)
    with open(input_file, "rb") as f_in:
        with open(output_file, "wb") as f_out:
            while True:
                chunk = f_in.read(8)
                if len(chunk) == 0:
                    break
                decrypted_chunk = des.decrypt(chunk)
                f_out.write(decrypted_chunk.rstrip(b" "))  # 去除填充的空格


# 主程序
if __name__ == "__main__":
    input_file = "plaintext.txt"  # 待加密的文件
    encrypted_file = "encrypted_file.des"
    decrypted_file = "decrypted_file.txt"
    key = get_random_bytes(8)  # 生成随机密钥

    # 加密文件
    encrypt_file(input_file, encrypted_file, key)
    print("File encrypted.")

    # 解密文件
    decrypt_file(encrypted_file, decrypted_file, key)
    print("File decrypted.")
