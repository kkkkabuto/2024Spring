def decrypt_vigenere(ciphertext, key):
    plaintext = ""
    key_index = 0
    for char in ciphertext:
        if char.isalpha():
            shift = (
                ord(key[key_index]) - ord("A")
                if key[key_index].isupper()
                else ord(key[key_index]) - ord("a")
            )
            if char.islower():
                plaintext += chr((ord(char) - ord("a") - shift) % 26 + ord("a"))
            else:
                plaintext += chr((ord(char) - ord("A") - shift) % 26 + ord("A"))
            key_index = (key_index + 1) % len(key)
        else:
            plaintext += char
    return plaintext


ciphertext = "CBXGU{ORF_BV_NVR_BLF_CRZL_QQ}"
key = "FDVNP"  # 这里是维吉尼亚密码的密钥

plaintext = decrypt_vigenere(ciphertext, key)
print("明文:", plaintext)
