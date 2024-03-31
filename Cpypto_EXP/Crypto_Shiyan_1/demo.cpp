#include <iostream>
#include <string>
#include <vector>
// ATTACKBEGINSATFIVE

int main()
{
	// 先进行Caesar密码加密解密操作
	std::string plaintext1;
	std::cout << "请输入要加密的明文：" << std::endl;
	std::cin >> plaintext1;

	// Caesar密码密钥
	int key1;
	std::cout << "请输入大小为0-25的密钥：" << std::endl;
	std::cin >> key1;
	std::string ciphertext1;

	// 加密
	std::cout << "明文经过Caesar密码加密后的密文是：" << std::endl;
    int flag = 1;
	for (char letter : plaintext1) {
		if (letter >= 'A' && letter <= 'Z') {
			int numericValue = letter - 'A';
			int transformedValue = (numericValue + key1) % 26;
			char transformedLetter = static_cast<char>(transformedValue + 'A');
			ciphertext1 = ciphertext1 + transformedLetter;
			std::cout << transformedLetter;
		}
        else if (letter >= 'a' && letter <= 'z') {
            int numericValue = letter - 'a';
            int transformedValue = (numericValue + key1) % 26;
            char transformedLetter = static_cast<char>(transformedValue + 'a');
            ciphertext1 = ciphertext1 + transformedLetter;
            std::cout << transformedLetter;
        }
		else
		{
            flag = 0;
			std::cout << "明文格式错误！！" << std::endl;
		}
	}

	// std::cout << ciphertext1 << std::endl;
	// 解密
    // 明文格式正确才解密
	std::cout << std::endl;
    if (flag == 1) {
        std::cout << "解密后的明文是：" << std::endl;
	        for (char letter : ciphertext1) {
                if (letter >= 'A' && letter <= 'Z') {
                    int numericValue = letter - 'A';
                    int transformedValue = (numericValue + 26 - key1) % 26;
                    char transformedLetter = static_cast<char>(transformedValue + 'A');
                    std::cout << transformedLetter;
                }
                else if(letter >= 'a' && letter <= 'z')
                {
                    int numericValue = letter - 'a';
                    int transformedValue = (numericValue + 26 - key1) % 26;
                    char transformedLetter = static_cast<char>(transformedValue + 'a');
                    std::cout << transformedLetter;
                }
		
	        }
    }
	

    // 置换密码
    std::cout << std::endl;
    std::string plaintext2;

    std::cout << "请输入要进行置换密码加密的明文：" << std::endl;
    std::cin >> plaintext2;

    // 由密钥字CIPHER得出列数为6
    int numColumns = 6;

    // 将明文按顺序排成 numColumns 列
    std::vector<std::string> columns(numColumns, "");
    for (size_t i = 0; i < plaintext2.length(); ++i) {
        columns[i % numColumns] += plaintext2[i];
    }

    // 直接定义 order 数组的值
    std::vector<int> order = { 0, 3, 4, 2, 1, 5 };  // CIPHER中各字母的顺序

    // 根据 order 数组的顺序重新排列列
    std::vector<std::string> newColums(numColumns, "");
    for (int i = 0; i < numColumns; ++i) {
        newColums[i] = columns[order[i]];
    }

    // 输出新矩阵
    std::cout << "新矩阵：" << std::endl;
    for (const std::string& column : newColums) {
        std::cout << column << std::endl;
    }

    // 输出 order
    std::cout << "Order 数组：" << std::endl;
    for (int val : order) {
        std::cout << val + 1 << " ";  // 输出从1开始的顺序
    }
    std::cout << std::endl;

    // 加密
    std::string ciphertext2;
    for (int j : {0, 3, 4, 2, 1, 5}) {
        for (int i = 0; i < newColums[0].length(); i++) {
            ciphertext2 += newColums[j][i];
        }
    }

    // 输出密文ciphertext2
    std::cout << "经置换加密后的密文是" << ciphertext2 << std::endl;

    // 解密
    std::vector<std::string> newColums2(numColumns, "");
    for (int i = 0; i < numColumns; ++i) {
        newColums2[order[i]] = newColums[i];
    }

    std::string plaintext3;
    for (int j = 0; j < newColums2[0].length(); j++) {
        for (int i = 0; i < numColumns; ++i) {
            plaintext3 += newColums2[i][j];
        }

    }

    // 输出明文plaintext3
    std::cout << "经解密的明文是" << plaintext3 << std::endl;

	return 0;
}