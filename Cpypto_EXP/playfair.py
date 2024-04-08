import re


def generate_playfair_matrix(key):
    key = key.replace(" ", "").upper()  # 去除空格并转换为大写
    key = re.sub(r"[^A-Z]", "", key)  # 保留密钥中的大写字母
    key = key.replace("J", "I")  # 将J替换为I
    key += "ABCDEFGHIKLMNOPQRSTUVWXYZ"  # 添加剩余字母
    key = "".join(dict.fromkeys(key))  # 去除重复字母
    matrix = [key[i : i + 5] for i in range(0, 25, 5)]  # 将字符串切分成5个字符一组
    return matrix


def encrypt(plaintext, key):
    matrix = generate_playfair_matrix(key)
    ciphertext = ""
    plaintext = re.sub(
        r"[^A-Z]", "", plaintext.upper()
    )  # 保留明文中的大写字母并转换为大写
    plaintext = re.sub(r"J", "I", plaintext)  # 将J替换为I
    plaintext = re.sub(
        r"(.)(?=\1)", r"\1X", plaintext
    )  # 如果两个字母相同，则在它们之间插入X
    if len(plaintext) % 2 == 1:  # 如果明文长度为奇数，则在末尾添加一个虚拟字符X
        plaintext += "X"
    for i in range(0, len(plaintext), 2):
        a, b = plaintext[i], plaintext[i + 1]
        a_row, a_col = divmod(matrix.index(a), 5)  # 获取字母a在矩阵中的位置
        b_row, b_col = divmod(matrix.index(b), 5)  # 获取字母b在矩阵中的位置
        if a_row == b_row:
            ciphertext += (
                matrix[a_row][(a_col + 1) % 5] + matrix[b_row][(b_col + 1) % 5]
            )  # 在同一行，每个字母向右移一位
        elif a_col == b_col:
            ciphertext += (
                matrix[(a_row + 1) % 5][a_col] + matrix[(b_row + 1) % 5][b_col]
            )  # 在同一列，每个字母向下移一位
        else:
            ciphertext += (
                matrix[a_row][b_col] + matrix[b_row][a_col]
            )  # 不在同一行或列，构成矩形，每个字母用对角线上的字母替换
    return ciphertext


def decrypt(ciphertext, key):
    matrix = generate_playfair_matrix(key)
    plaintext = ""
    ciphertext = re.sub(
        r"[^A-Z]", "", ciphertext.upper()
    )  # 保留密文中的大写字母并转换为大写
    for i in range(0, len(ciphertext), 2):
        a, b = ciphertext[i], ciphertext[i + 1]
        a_row, a_col = divmod(matrix.index(a), 5)  # 获取字母a在矩阵中的位置
        b_row, b_col = divmod(matrix.index(b), 5)  # 获取字母b在矩阵中的位置
        if a_row == b_row:
            plaintext += (
                matrix[a_row][(a_col - 1) % 5] + matrix[b_row][(b_col - 1) % 5]
            )  # 在同一行，每个字母向左移一位
        elif a_col == b_col:
            plaintext += (
                matrix[(a_row - 1) % 5][a_col] + matrix[(b_row - 1) % 5][b_col]
            )  # 在同一列，每个字母向上移一位
        else:
            plaintext += (
                matrix[a_row][b_col] + matrix[b_row][a_col]
            )  # 不在同一行或列，构成矩形，每个字母用对角线上的字母替换
    return plaintext


# 测试加密和解密功能
key = "PLAYFAIREXAMPLE"
plaintext = "HELLO WORLD"
ciphertext = encrypt(plaintext, key)
print("加密后的密文:", ciphertext)
decrypted_plaintext = decrypt(ciphertext, key)
print("解密后的明文:", decrypted_plaintext)
