#include <iostream>
#include <string>
#include <vector>
// ATTACKBEGINSATFIVE

int main()
{
	// �Ƚ���Caesar������ܽ��ܲ���
	std::string plaintext1;
	std::cout << "������Ҫ���ܵ����ģ�" << std::endl;
	std::cin >> plaintext1;

	// Caesar������Կ
	int key1;
	std::cout << "�������СΪ0-25����Կ��" << std::endl;
	std::cin >> key1;
	std::string ciphertext1;

	// ����
	std::cout << "���ľ���Caesar������ܺ�������ǣ�" << std::endl;
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
			std::cout << "���ĸ�ʽ���󣡣�" << std::endl;
		}
	}

	// std::cout << ciphertext1 << std::endl;
	// ����
    // ���ĸ�ʽ��ȷ�Ž���
	std::cout << std::endl;
    if (flag == 1) {
        std::cout << "���ܺ�������ǣ�" << std::endl;
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
	

    // �û�����
    std::cout << std::endl;
    std::string plaintext2;

    std::cout << "������Ҫ�����û�������ܵ����ģ�" << std::endl;
    std::cin >> plaintext2;

    // ����Կ��CIPHER�ó�����Ϊ6
    int numColumns = 6;

    // �����İ�˳���ų� numColumns ��
    std::vector<std::string> columns(numColumns, "");
    for (size_t i = 0; i < plaintext2.length(); ++i) {
        columns[i % numColumns] += plaintext2[i];
    }

    // ֱ�Ӷ��� order �����ֵ
    std::vector<int> order = { 0, 3, 4, 2, 1, 5 };  // CIPHER�и���ĸ��˳��

    // ���� order �����˳������������
    std::vector<std::string> newColums(numColumns, "");
    for (int i = 0; i < numColumns; ++i) {
        newColums[i] = columns[order[i]];
    }

    // ����¾���
    std::cout << "�¾���" << std::endl;
    for (const std::string& column : newColums) {
        std::cout << column << std::endl;
    }

    // ��� order
    std::cout << "Order ���飺" << std::endl;
    for (int val : order) {
        std::cout << val + 1 << " ";  // �����1��ʼ��˳��
    }
    std::cout << std::endl;

    // ����
    std::string ciphertext2;
    for (int j : {0, 3, 4, 2, 1, 5}) {
        for (int i = 0; i < newColums[0].length(); i++) {
            ciphertext2 += newColums[j][i];
        }
    }

    // �������ciphertext2
    std::cout << "���û����ܺ��������" << ciphertext2 << std::endl;

    // ����
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

    // �������plaintext3
    std::cout << "�����ܵ�������" << plaintext3 << std::endl;

	return 0;
}