import random
from Crypto.Util.number import *

n = 123123

p = int("".join([random.choice("123") for _ in range(512)]))
print(p)
