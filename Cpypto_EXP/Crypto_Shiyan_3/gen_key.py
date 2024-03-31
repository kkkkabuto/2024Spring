# DES子密钥的生成算法，并根据自己设定的种子密钥运行程序产生并显示出16轮的子密钥，同时完成DES算法弱秘钥和半弱秘钥对的验证
# 初始的 56 位密钥
initial_key = [
    0x11223344556677FE,
    0x0101010101010101,
    0x1F1F1F1F0F0F0F0F,
    0xE0E0E0E0F1F1F1F1,
    0xFEFEFEFEFEFEFEFE,
    0x01FE01FE01FE01FE,
    0xFE01FE01FE01FE01,
    0x01E001E001F101F1,
    0xE001E001F101F101,
]

# 每一轮左移的位数
shifts = [1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1]

# PC-1 置换表
pc1_table = [
    57,
    49,
    41,
    33,
    25,
    17,
    9,
    1,
    58,
    50,
    42,
    34,
    26,
    18,
    10,
    2,
    59,
    51,
    43,
    35,
    27,
    19,
    11,
    3,
    60,
    52,
    44,
    36,
    63,
    55,
    47,
    39,
    31,
    23,
    15,
    7,
    62,
    54,
    46,
    38,
    30,
    22,
    14,
    6,
    61,
    53,
    45,
    37,
    29,
    21,
    13,
    5,
    28,
    20,
    12,
    4,
]

# PC-2 置换表
pc2_table = [
    14,
    17,
    11,
    24,
    1,
    5,
    3,
    28,
    15,
    6,
    21,
    10,
    23,
    19,
    12,
    4,
    26,
    8,
    16,
    7,
    27,
    20,
    13,
    2,
    41,
    52,
    31,
    37,
    47,
    55,
    30,
    40,
    51,
    45,
    33,
    48,
    44,
    49,
    39,
    56,
    34,
    53,
    46,
    42,
    50,
    36,
    29,
    32,
]


# 生成子密钥
def generate_subkeys(key):
    # PC-1 置换
    key = permute(key, pc1_table, 56, 64)
    print(key)
    # 分割成左右两部分
    c0 = key >> 28
    d0 = key & 0xFFFFFFF
    subkeys = []

    # 生成 16 个子密钥
    for shift in shifts:
        # 左移操作
        c0 = ((c0 << shift) & 0x0FFFFFFF) | (c0 >> (28 - shift))
        d0 = ((d0 << shift) & 0x0FFFFFFF) | (d0 >> (28 - shift))

        # print(c0, d0)
        # 合并并进行 PC-2 置换
        subkey = permute((c0 << 28) | d0, pc2_table, 48, 56)
        subkeys.append(subkey)

    return subkeys


# 置换函数，size2用于取出table[i]数字对应在置换前的那一位数字，PC-1时为64，PC-2时为56
def permute(data, table, size, size2):
    permuted_data = 0
    for i in range(size):
        permuted_data |= ((data >> (size2 - table[i])) & 1) << (size - i - 1)
    return permuted_data


# 打印生成的子密钥

# 控制台打印16轮子密钥
# for k in range(len(initial_key)):
#     subkeys = generate_subkeys(initial_key[k])
#     print(f"initial_key{k} :")
#     for i, subkey in enumerate(subkeys, 1):
#         # print(f"initial_key{k} :")
#         print(f"Subkey {i}: {hex(subkey)}")

# 将密钥写入out.txt中方便查看
# 打开文件并清空内容
with open("out.txt", "w") as f:
    f.write("")

with open("out.txt", "w") as f:
    for k in range(len(initial_key)):
        subkeys = generate_subkeys(initial_key[k])
        f.write(f"initial_key{k} :\n")
        for i, subkey in enumerate(subkeys, 1):
            f.write(f"Subkey {i}: {hex(subkey)}\n")
