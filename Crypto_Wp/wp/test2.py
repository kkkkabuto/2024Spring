import gmpy2
from Crypto.Util.number import *
from Crypto.PublicKey import RSA
import os

import gmpy2
import libnum
from Crypto.Util.number import long_to_bytes


def transform(x, y):  # 使用辗转相除将分数 x/y 转为连分数的形式
    res = []
    while y:
        res.append(x // y)
        x, y = y, x % y
    return res


def continued_fraction(sub_res):
    numerator, denominator = 1, 0
    for i in sub_res[::-1]:  # 从sublist的后面往前循环
        denominator, numerator = numerator, i * numerator + denominator
    return denominator, numerator  # 得到渐进分数的分母和分子，并返回


# 求解每个渐进分数
def sub_fraction(x, y):
    res = transform(x, y)
    res = list(
        map(continued_fraction, (res[0:i] for i in range(1, len(res))))
    )  # 将连分数的结果逐一截取以求渐进分数
    return res


def get_pq(a, b, c):  # 由p+q和pq的值通过韦达定理来求解p和q
    par = gmpy2.isqrt(b * b - 4 * a * c)  # 由上述可得，开根号一定是整数，因为有解
    x1, x2 = (-b + par) // (2 * a), (-b - par) // (2 * a)
    return x1, x2


def wienerAttack(e, n):
    for d, k in sub_fraction(
        e, n
    ):  # 用一个for循环来注意试探e/n的连续函数的渐进分数，直到找到一个满足条件的渐进分数
        if k == 0:  # 可能会出现连分数的第一个为0的情况，排除
            continue
        if (
            e * d - 1
        ) % k != 0:  # ed=1 (mod φ(n)) 因此如果找到了d的话，(ed-1)会整除φ(n),也就是存在k使得(e*d-1)//k=φ(n)
            continue

        phi = (e * d - 1) // k  # 这个结果就是 φ(n)
        px, qy = get_pq(1, n - phi + 1, n)
        if px * qy == n:
            p, q = abs(int(px)), abs(
                int(qy)
            )  # 可能会得到两个负数，负负得正未尝不会出现
            d = gmpy2.invert(
                e, (p - 1) * (q - 1)
            )  # 求ed=1 (mod  φ(n))的结果，也就是e关于 φ(n)的乘法逆元d
            p = gmpy2.iroot(p, 2)[0]
            q = gmpy2.iroot(q, 2)[0]
            return d, p, q
    print("该方法不适用")


e = 4602579741478096718172697218991734057017874575484294836043557658035277770732473025335441717904100009903832353915404911860888652406859201203199117870443451616457858224082143505393843596092945634675849883286107358454466242110831071552006337406116884147391687266536283395576632885877802269157970812862013700574069981471342712011889330292259696760297157958521276388120468220050600419562910879539594831789625596079773163447643235584124521162320450208920533174722239029506505492660271016917768383199286913178821124229554263149007237679675898370759082438533535303763664408320263258144488534391712835778283152436277295861859
n = 78665180675705390001452176028555030916759695827388719494705803822699938653475348982551790040292552032924503104351703419136483078949363470430486531014134503794074329285351511023863461560882297331218446027873891885693166833003633460113924956936552466354566559741886902240131031116897293107970411780310764816053
d, p, q = wienerAttack(e, n * n)
assert p * q == n
import hashlib

flag = "qsnctf{" + hashlib.md5(str(p + q).encode()).hexdigest() + "}"
print(flag)
# qsnctf{8072e8b2982bc729cc74ef58f1abc862}
