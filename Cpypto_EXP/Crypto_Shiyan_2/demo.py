from PIL import Image
import numpy as np


def arnold_transform(image, iterations):
    width, height = image.size
    pixels = np.array(image)

    a, b = 1, 1  # 调整参数，可以根据需要进行调整

    for _ in range(iterations):
        new_pixels = np.zeros_like(pixels)

        for x in range(width):
            for y in range(height):
                new_x = (x + a * y) % width
                new_y = (b * x + (a * b + 1) * y) % height

                new_pixels[new_x, new_y] = pixels[x, y]

        pixels = new_pixels

    return Image.fromarray(pixels)


def arnold_reverse(image, iterations):
    width, height = image.size
    pixels = np.array(image)

    a, b = 1, 1  # 调整参数，与变换时相同

    for _ in range(iterations):
        new_pixels = np.zeros_like(pixels)

        for x in range(width):
            for y in range(height):
                new_x = (x - a * y) % width
                new_y = ((-b) * x + (a * b + 1) * y) % height

                new_pixels[new_x, new_y] = pixels[x, y]

        pixels = new_pixels

    return Image.fromarray(pixels)


# 读取图像
original_image = Image.open("2.jpg")

# 进行Arnold变换
iterations = 50  # 调整迭代次数,图片尺寸是100*100，迭代150次还原
encrypted_image = arnold_transform(original_image, iterations)

# 保存置乱后的图像
encrypted_image.save("encrypted_image.jpg")

# 恢复图像
decrypted_image = arnold_reverse(encrypted_image, iterations)

# 保存恢复后的图像
decrypted_image.save("decrypted_image.jpg")
