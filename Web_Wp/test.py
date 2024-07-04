def decrypt(text):
    """
    # 此函数将在如下代码执行时进行调用
    path = request.path.strip('/')
    解密路径
    decrypted_path = decrypt(path.encode())
    """
    cipher = AES.new(key, AES.MODE_ECB)
    try:
        decrypted_text = cipher.decrypt(binascii.unhexlify(text))
        return decrypted_text.rstrip(b' ').decode()
    except Exception as e:
        source_code = inspect.getsource(decrypt)
        # return render_template_string(f"Error: {str(e)}Decrypt function source code:%s " %  source_code)
        error_message = f"Error: {str(e)}