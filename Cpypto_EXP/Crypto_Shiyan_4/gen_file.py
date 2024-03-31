import random
import string

# 定义生成的文件大小（字节）
file_size = 1024 * 1024 * 1024  # 1MB

# 生成1MB大小的随机文本
random_text = "".join(random.choices(string.ascii_letters + string.digits, k=file_size))

# 将生成的文本写入文件
with open("file_GB.txt", "w") as f:
    f.write(random_text)
