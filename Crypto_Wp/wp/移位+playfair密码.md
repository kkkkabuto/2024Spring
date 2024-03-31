# 题目
- ctfshow justshow
```
hlcgoyfsjqknyifnpd:bcdefghijklmnopqrstuvwxyza
```

# 解题
冒号后面的意思是移位密码
加密算法数c = (m + 1) mod 26
冒号前面是密文
先解密移位密码
```python
s = "hlcgoyfsjqknyifnpd"
t = ""
for i in s:
    t += chr(ord(i) - 1)
print(t)
```
得到字符串
```
gkbfnxeripjmxhemoc
```

接下来使用playfair密码解密,密钥是abcdefghijklmnopqrstuvwxyz
http://www.hiencode.com/playfair.html
得到
```
flagisctfshowicome
```

# flag
```
flag{ctfshowicome}
```
