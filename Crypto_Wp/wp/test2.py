v4 = [
    37,
    40,
    36,
    33,
    60,
    42,
    60,
    30,
    20,
    40,
    36,
    40,
    41,
    97,
    50,
    39,
    63,
    32,
    12,
    9,
    32,
    104,
    55,
    46,
    4,
    63,
    53,
    106,
    17,
    7,
    4,
    61,
    14,
    17,
    38,
    14,
    26,
]

v8 = 67
flag = ""
for value in v4:
    for i in range(256):
        if i ^ v8 == value:
            flag += chr(i)
    v8 += 1

print(flag)

# flag{buT_diff1cultY_w0nt_ch4Nge_muCh}
