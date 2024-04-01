from Crypto.Util.number import *
import base64

# 这里是被隐写的base64密文
c = b"VEhJUz==\nRkxBR3==\nSVN=\nSElEREVOLo==\nQ0FO\nWU9V\nRklORM==\nSVT=\nT1VUP4==\nRE8=\nWU9V\nS05PV9==\nQkFTRTY0P5==\nWW91bmdD\nVEhJTku=\nWU9V\nQVJF\nTk9U\nVEhBVE==\nRkFNSUxJQVI=\nV0lUSO==\nQkFTRTY0Lh==\nQmFzZTY0\naXO=\nYW==\nZ3JvdXA=\nb2b=\nc2ltaWxhcn==\nYmluYXJ5LXRvLXRleHR=\nZW5jb2Rpbme=\nc2NoZW1lc0==\ndGhhdD==\ncmVwcmVzZW50\nYmluYXJ5\nZGF0YW==\naW5=\nYW6=\nQVNDSUl=\nc3RyaW5n\nZm9ybWF0\nYnk=\ndHJhbnNsYXRpbmd=\naXS=\naW50b1==\nYT==\ncmFkaXgtNjQ=\ncmVwcmVzZW50YXRpb24u\nVGhl\ndGVybc==\nQmFzZTY0\nb3JpZ2luYXRlc8==\nZnJvbd==\nYY==\nc3BlY2lmaWN=\nTUlNRT==\nY29udGVudI==\ndHJhbnNmZXI=\nZW5jb2Rpbmcu\nVGhl\ncGFydGljdWxhct==\nc2V0\nb2b=\nNjR=\nY2hhcmFjdGVyc5==\nY2hvc2Vu\ndG+=\ncmVwcmVzZW50\ndGhl\nNjQ=\ncGxhY2UtdmFsdWVz\nZm9y\ndGhl\nYmFzZd==\ndmFyaWVz\nYmV0d2Vlbt==\naW1wbGVtZW50YXRpb25zLp==\nVGhl\nZ2VuZXJhbI==\nc3RyYXRlZ3n=\naXO=\ndG9=\nY2hvb3Nl\nNjR=\nY2hhcmFjdGVyc5==\ndGhhdA==\nYXJl\nYm90aN==\nbWVtYmVyc5==\nb2a=\nYS==\nc3Vic2V0\nY29tbW9u\ndG8=\nbW9zdM==\nZW5jb2RpbmdzLA==\nYW5k\nYWxzb8==\ncHJpbnRhYmxlLg==\nVGhpc9==\nY29tYmluYXRpb25=\nbGVhdmVz\ndGhl\nZGF0YW==\ndW5saWtlbHk=\ndG/=\nYmV=\nbW9kaWZpZWS=\naW5=\ndHJhbnNpdE==\ndGhyb3VnaN==\naW5mb3JtYXRpb26=\nc3lzdGVtcyw=\nc3VjaN==\nYXM=\nRS1tYWlsLD==\ndGhhdA==\nd2VyZQ==\ndHJhZGl0aW9uYWxseQ==\nbm90\nOC1iaXQ=\nY2xlYW4uWzFd\nRm9y\nZXhhbXBsZSw=\nTUlNRSdz\nQmFzZTY0\naW1wbGVtZW50YXRpb24=\ndXNlcw==\nQahDWiw=\nYahDeiw=\nYW5k\nMKhDOQ==\nZm9y\ndGhl\nZmlyc3Q=\nNjI=\ndmFsdWVzLg==\nT3RoZXI=\ndmFyaWF0aW9ucw==\nc2hhcmU=\ndGhpcw==\ncHJvcGVydHk=\nYnV0\nZGlmZmVy\naW4=\ndGhl\nc3ltYm9scw==\nY2hvc2Vu\nZm9y\ndGhl\nbGFzdA==\ndHdv\ndmFsdWVzOw==\nYW4=\nZXhhbXBsZQ==\naXM=\nVVRGLTcu"


def get_base64_diff_value(s1, s2):
    base64chars = b"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
    res = 0
    for i in range(len(s2)):
        if s1[i] != s2[i]:
            return abs(base64chars.index(s1[i]) - base64chars.index(s2[i]))
    return res


def solve_stego():
    line = b""
    bin_str = ""
    for i in c:
        k = long_to_bytes(i)
        if k == b"\n":
            steg_line = line
            norm_line = base64.b64encode(base64.b64decode(line))
            diff = get_base64_diff_value(steg_line, norm_line)
            # print(diff)
            pads_num = steg_line.count(b"=")
            if diff:
                bin_str += bin(diff)[2:].zfill(pads_num * 2)
            else:
                bin_str += "0" * pads_num * 2
            print(goflag(bin_str))
            line = b""
            continue
        line += k


def goflag(bin_str):
    res_str = ""
    for i in range(0, len(bin_str), 8):
        res_str += chr(int(bin_str[i : i + 8], 2))
    return res_str


if __name__ == "__main__":
    solve_stego()
