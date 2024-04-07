from Cryptodome.Cipher import AES
import binascii

msg = b"0000000000000000"
cip = b"8e961f91840bab8b9cacd87b28102f66"

# 读取 dictionary.txt 中的所有拼音
pinyins = "zhao qian sun li zhou wu zheng wang feng chen chu wei jiang chen han yang zhu qin you xu he lv shi zhang kong cao yan hua jin wei tao jiang qi xie zou yu bai shui dou zhang yun su pan ge xi fan peng lang lu wei chang ma miao feng hua fang yu ren yuan liu feng bao shi tang fei lian cen xue lei he ni tang teng yin luo bi hao wu an chang le yu shi fu pi bian qi kang wu yu yuan bu gu meng ping huang he mu xiao yin yao shao zhan wang qi mao yu di mi bei ming zang ji fu cheng dai tan song mao pang xiong ji shu qu xiang zhu dong liang du ruan lan min xi ji ma qiang jia lu lou wei jiang tong yan guo mei sheng lin diao zhong xu qiu luo gao xia cai tian fan hu ling huo yu wan zhi ke zan guan lu mo jing fang qiu mou gan jie ying zong ding xuan bi deng yu dan hang hong bao zhu zuo shi cui ji niu gong cheng ji xing hua pei lu rong weng xun yang yu hui zhen qu jia feng rui yi chu jin ji bing mi song jing duan fu wu wu jiao ba gong mu wei shan gu che hou mi peng quan chi ban yang qiu zhong yi gong ning chou luan bao gan xie li rong zu wu fu liu jing zhan shu long xie xing si shao gao li ji bo yin su bai huai pu tai cong e suo xian ji lai zhuo lin tu meng chi qiao yin yu xu neng cang shuang wen shen dang di tan gong lao pang ji shen fu du ran zai li yong xi qu sang gui pu niu shou tong bian hu yan ji jia pu shang nong wen bie zhuang yan chai ju yan chong mu lian ru xi huan ai yu rong xiang gu yi shen ge liao yu zhong ji ju heng bu du geng man hong kuang guo wen kou guang lu que dong ou shu wo li wei yue kui long shi gong she nie chao gou ao rong leng zi xin kan na jian rao kong ceng wu sha mie yang ju xu feng chao guan kuai xiang cha hou jing hong you zhu quan qiu gai yi huan gong zhang du jin chu yan fa ru yan tu qin yue shuai gou kang kuang hou you qin gui hai mo ha qiao da nian ai yang tong shang mou she er bo shang "
# 定义生日范围
birthdays = [(year, month, day) for year in range(1900, 2100) for month in range(1, 13) for day in range(1, 32)]

# 遍历生日和拼音的组合
for birthday in birthdays:
    for pinyin in pinyins.split():
        # 拼音和日期组合在一起作为密钥
        key = f"{pinyin}{birthday[0]:04d}{birthday[1]:02d}{birthday[2]:02d}"
        key += '0' * (16 - len(key))
        key = key[:16]
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

        continue  # 如果内部循环未找到密钥，则继续外部循环
    break  # 如果找到了密钥，则退出外部循环
else:
    print("Key not found")
