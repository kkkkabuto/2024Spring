from Cryptodome.Cipher import AES
import binascii
from concurrent.futures import ThreadPoolExecutor

msg = b'0000000000000000'
cip = b'df757f188880619e7b6f5e69b498ae42'

# 给定的字符串
given_string="zhao qian sun li zhou wu zheng wang feng chen chu wei jiang chen han yang zhu qin you xu he lv shi zhang kong cao yan hua jin wei tao jiang qi xie zou yu bai shui dou zhang yun su pan ge xi fan peng lang lu wei chang ma miao feng hua fang yu ren yuan liu feng bao shi tang fei lian cen xue lei he ni tang teng yin luo bi hao wu an chang le yu shi fu pi bian qi kang wu yu yuan bu gu meng ping huang he mu xiao yin yao shao zhan wang qi mao yu di mi bei ming zang ji fu cheng dai tan song mao pang xiong ji shu qu xiang zhu dong liang du ruan lan min xi ji ma qiang jia lu lou wei jiang tong yan guo mei sheng lin diao zhong xu qiu luo gao xia cai tian fan hu ling huo yu wan zhi ke"

def attack_key(word):
    for shuzi in range(0, 100000000):  # 年份范围设定在 0 到 99999999 之间
        key = f"{word}{shuzi:08d}"
        key += '0' * (16 - len(key))  # 补零以使密钥长度为 16 字节
        key = key[:16]  # 如果密钥长度超过 16 字节，则截取前 16 个字符
        key_bytes = key.encode('utf-8')

        aes = AES.new(key_bytes, AES.MODE_ECB)
        encrypted_text = aes.encrypt(msg)
        cipher = binascii.b2a_hex(encrypted_text)

        print(key)

        if cip == cipher:
            print("Key Found:", key)
            return key
    return None

# 创建 ThreadPoolExecutor
with ThreadPoolExecutor(max_workers=16) as executor:
    # 提交任务
    futures = [executor.submit(attack_key, word) for word in given_string.split()]

    # 等待任务完成
    for future in futures:
        result = future.result()
        if result:
            break

    if not result:
        print("Key not found")
