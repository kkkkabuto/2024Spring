# 题目
- ctfshow 36D杯 签到

```
W9@F0>:2?0D9:07=28X/3/TUW/o/7/PUo/ST7/T/6/R
```

# 解题
题目提示rot，那就rot解密 
https://www.qqxiuzi.cn/bianma/ROT5-13-18-47.php

rot47,18，13,5都试一下，rot47结果如下
```
(hou_mian_shi_flag)^b^%&(^@^f^!&@^$%f^%^e^#
```

题目提示rot之后看键盘，键盘密码
```
^b^%&(^@^f^!&@^$%f^%^e^#
```
对应
```
6b6579626f6172645f656e63
```

16进制转字符串，得到
```
keyboard_enc
```

# flag
```
flag{keyboard_enc}
```

