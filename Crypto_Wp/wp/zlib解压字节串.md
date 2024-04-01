- 学到一个新东西
- 2018网鼎杯 track_hacker
- 查看tcp流末尾发现eJxLy0lMrw6NTzPMS4n3TVWsBQAz4wXi
- 在线base64解密得到乱码
- python解密：
```python
import base64, zlib

flag = "eJxLy0lMrw6NTzPMS4n3TVWsBQAz4wXi"
print(base64.b64decode(flag))
# b'x\x9cK\xcbIL\xaf\x0e\x8dO3\xccK\x89\xf7MU\xac\x05\x003\xe3\x05\xe2'
```
- 得到的是字节串
- zlib字节压缩：
```python
import base64, zlib

flag = "eJxLy0lMrw6NTzPMS4n3TVWsBQAz4wXi"
print(base64.b64decode(flag))
print(zlib.decompress(base64.b64decode(flag)))
# b'flag{U_f1nd_Me!}'
```