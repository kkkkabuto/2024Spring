# Diffie-Hellman密钥交换
- uvw77ay_aiffip
```
小明听说某软件会自动备份自己的聊天记录，非常恼火，一怒之下设计了个超级基础协商协议。某天他准备和你通讯，发给你了一段协商消息，附件中还有公用生成元g和你的私钥，以及保证密钥长度的质数。那么，你们的最后协商的32位密钥是啥呢？（题目标题有玄机，答案为32位的十进制数

你们共同有一个公开已知生成元g=34

你的私钥=9360

小明向你发来协商数据：57687185873717832710418532982088

保证长度的质数为：72542982052896163041354492265957


```

- EXP:
```python
# 公开已知的生成元和保证长度的质数
g = 34
prime = 72542982052896163041354492265957

# 你的私钥和小明的协商数据
your_private_key = 9360
shared_data_from_ming = 57687185873717832710418532982088

# 计算你的公钥
your_public_key = pow(g, your_private_key, prime)

# 小明计算共享密钥
shared_key_from_ming = pow(shared_data_from_ming, your_private_key, prime)

print("你的公钥:", your_public_key)
print("小明计算的共享密钥:", shared_key_from_ming)
# flag{37799067929880674766269861874334}
```