# 波利比奥斯方阵密码Polybius
- [BJDCTF2020]Polybius
- 解题可以参考 https://www.cnblogs.com/labster/p/13842837.html
```
密文：ouauuuoooeeaaiaeauieuooeeiea
hint：VGhlIGxlbmd0aCBvZiB0aGlzIHBsYWludGV4dDogMTQ=
flag:解出明文后，请加上BJD{}
```
- 解密base64字符串得到The length of this plaintext: 14
- 也就是说明文字符串长度是14，而密文字符串长度是28，每个明文字符被两个密文字符替代
- 观察密文发现只有aeiou五个字符，问题是不知道排列顺序，所以需要试5*4*3*2*1种情况，i和j又是在同一个位置，还需要乘以2，总共需要试240种可能，然后寻找有意义的字符串。但是脚本中只试了120种，遇到提交flag不正确就把i和j替换
```python
import itertools

cipher = "ouauuuoooeeaaiaeauieuooeeiea"
k = "aeiou"
k_list = []  # 记录所有的排列
num_k_list = []  # 记录密文对应每一种排列的数字，元素个数和k_list相同
for i in itertools.permutations(k, 5):
    k_list.append("".join(i))
# print(format(k_list))

for i in k_list:
    temp = ""
    for j in cipher:
        temp += str(i.index(j) + 1)
    num_k_list.append(temp)
# print(len(num_k_list))
for i in num_k_list:
    res = ""
    for j in range(0, len(i), 2):
        # (int(i[j])是行数，int(i[j + 1])是列数
        temp = (
            (int(i[j]) - 1) * 5 + int(i[j + 1]) + 96
        )  # 加96是因为i，j在同一个位置，后面再判断是否加1即可
        if temp > ord("i"):
            temp += 1
        res += chr(temp)
    print(res)

# flag{flagispolybius}
```
