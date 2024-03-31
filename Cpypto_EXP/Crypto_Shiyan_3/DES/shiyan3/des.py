from Crypto.Cipher import DES
from Crypto.Random import get_random_bytes

def generate_subkeys(seed_key):
    # 根据种子密钥生成子密钥
    subkeys = []
    key = seed_key
    for round_num in range(16):
        subkeys.append(key)
        key = generate_next_subkey(key, round_num)
    return subkeys

def generate_next_subkey(key, round_num):
    # 生成下一轮的子密钥
    key_bin = bin(int(key, 16))[2:].zfill(64)  # 将16进制的密钥转为64位的二进制
    left_half = key_bin[:28]
    right_half = key_bin[28:]

    # 根据循环位移表，进行循环位移
    shift_amount = DES_KEY_SHIFT_AMOUNTS[round_num]
    left_half = left_half[shift_amount:] + left_half[:shift_amount]
    right_half = right_half[shift_amount:] + right_half[:shift_amount]

    # 合并左右半部分，生成下一轮的子密钥
    next_subkey_bin = left_half + right_half
    next_subkey_hex = hex(int(next_subkey_bin, 2))[2:].zfill(16)
    return next_subkey_hex.upper()

# DES循环位移表
DES_KEY_SHIFT_AMOUNTS = [
    1, 1, 2, 2, 2, 2, 2, 2,
    1, 2, 2, 2, 2, 2, 2, 1
]

def verify_weak_keys():
    # 弱秘钥
    weak_key = "01010101010101011F1F1F1F0F0F0E0EE0E0E0E0F1F1F1F1FEFEFEFEFEFEFEFE"
    is_weak = verify_key_pair(weak_key)
    print(f"Is weak key: {is_weak}")

    # 半弱密钥对
    weak_key_pair = ["01FE01FE01FE01FEFE01FE01FE01FE01", "01E001E001F101F1E001E001F101F101"]
    for key in weak_key_pair:
        is_weak = verify_key_pair(key)
        print(f"Is weak key: {is_weak}")

def verify_key_pair(key):
    # 校验DES弱秘钥
    des = DES.new(bytes.fromhex(key), DES.MODE_ECB)
    plaintext = b"A simple message"  # 任意明文
    ciphertext = des.encrypt(plaintext)
    decrypted_text = des.decrypt(ciphertext)
    return plaintext == decrypted_text

if __name__ == "__main__":
    seed_key = "0011223344556677"  # 设定的种子密钥
    subkeys = generate_subkeys(seed_key)

    # 输出16轮的子密钥
    print("Subkeys:")
    for i, subkey in enumerate(subkeys):
        print(f"Round {i + 1}: {subkey}")

    verify_weak_keys()
    # 初始密钥置换表
    PC1 = [
    57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18,
    10, 2, 59, 51, 43, 35, 27, 19, 11, 3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15, 7, 62, 54, 46, 38, 30, 22,
    14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 28, 20, 12, 4
    ]

    # 循环左移位数表，16轮
    SHIFT_SCHEDULE = [
    1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
    ]

    # 压缩置换表
    PC2 = [
    14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10, 23, 19, 12, 4,
    26, 8, 16, 7, 27, 20, 13, 2, 41, 52, 31, 37, 47, 55, 30, 40,
    51, 45, 33, 48, 44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32
]

    def initial_permutation(key):
        return [key[bit - 1] for bit in PC1]

    def left_shift(key, shift):
        return key[shift:] + key[:shift]

    def compression_permutation(key):
        return [key[bit - 1] for bit in PC2]

    def generate_subkeys(seed_key):
        key_bits = [int(bit) for bit in bin(seed_key)[2:].zfill(64)]  # 将密钥转换为二进制列表
        permuted_key = initial_permutation(key_bits)

        subkeys = []
        left, right = permuted_key[:28], permuted_key[28:]

        for round_shift in SHIFT_SCHEDULE:
            left = left_shift(left, round_shift)
            right = left_shift(right, round_shift)
            subkey = compression_permutation(left + right)
            subkeys.append(subkey)

        return subkeys

    # 设定的种子密钥
    seed_key = 0x01010101010101011F1F1F1F0F0F0E0EE0E0E0E0F1F1F1F1FEFEFEFEFEFEFEFE
    subkeys = generate_subkeys(seed_key)

    # 打印生成的16轮子密钥
    for i, subkey in enumerate(subkeys, 1):
        print(f"Round {i}: {hex(int(''.join(map(str, subkey)), 2))}")

    # 弱密钥
    weak_key1 = 0x0101010101010101
    weak_key2 = 0xFEFEFEFEFEFEFEFE

    # 半弱密钥对
    semi_weak_key1 = 0x01FE01FE01FE01FE
    semi_weak_key2 = 0xFE01FE01FE01FE01

    # 打印验证结果
    print("\nWeak Key 1:  ", seed_key == weak_key1)
    print("Weak Key 2:  ", seed_key == weak_key2)
    print("Semi-Weak Key Pair 1:  ", seed_key == semi_weak_key1)
    print("Semi-Weak Key Pair 2:  ", seed_key == semi_weak_key2)
