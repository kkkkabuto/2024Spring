//#include <iostream>
//#include <string>
//#include <vector>
//#include <algorithm>
//#include <numeric>
//// ATTACKBEGINSATFIVE
//
//int main()
//{
//    // 置换密码
//    std::cout << std::endl;
//    std::string plaintext2;
//
//    std::cout << "请输入要进行置换密码加密的明文：" << std::endl;
//    std::cin >> plaintext2;
//
//    // 由密钥字CIPHER得出列数为6
//    int numColumns = 6;  
//
//    // 将明文按顺序排成 numColumns 列
//    std::vector<std::string> columns(numColumns, "");
//    for (size_t i = 0; i < plaintext2.length(); ++i) {
//        columns[i % numColumns] += plaintext2[i];
//    }
//
//    // 直接定义 order 数组的值
//    std::vector<int> order = { 0, 3, 4, 2, 1, 5 };  // CIPHER中各字母的顺序
//
//    // 根据 order 数组的顺序重新排列列
//    std::vector<std::string> newColums(numColumns, "");
//    for (int i = 0; i < numColumns; ++i) {
//        newColums[i] = columns[order[i]];
//    }
//
//    // 输出新矩阵
//    std::cout << "新矩阵：" << std::endl;
//    for (const std::string& column : newColums) {
//        std::cout << column << std::endl;
//    }
//
//    // 输出 order
//    std::cout << "Order 数组：" << std::endl;
//    for (int val : order) {
//        std::cout << val + 1 << " ";  // 输出从1开始的顺序
//    }
//    std::cout << std::endl;
//
//    // 加密
//    std::string ciphertext2;
//    for (int j : {0, 3, 4, 2, 1, 5}) {
//        for (int i = 0; i < newColums[0].length(); i++) {
//            ciphertext2 += newColums[j][i];
//        }
//    }
//
//    // 输出密文ciphertext2
//    std::cout << "经置换加密后的密文是" << ciphertext2 << std::endl;
//
//    // 解密
//    std::vector<std::string> newColums2(numColumns, "");
//    for (int i = 0; i < numColumns; ++i) {
//        newColums2[order[i]] = newColums[i];
//    }
//
//    std::string plaintext3;
//    for (int j = 0; j < newColums2[0].length(); j++) {
//        for (int i = 0; i < numColumns; ++i) {
//            plaintext3 += newColums2[i][j];
//        }
//        
//    }
//
//    // 输出明文plaintext3
//    std::cout << "经解密的明文是" << plaintext3 << std::endl;
//
//    return 0;
//}
//
//
//
//
//
//
//
//
