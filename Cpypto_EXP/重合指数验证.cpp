#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>

using namespace std;

int main() {
    // �����ַ���
    string ciphertext =
        "CHREEVOAHMAERATBIAXXWTNXBEEOPHBSBQMQEQERBW"
        "RVXUOAKXAOSXXWEAHBWGJMMQMNKGRFVGXWTRZXWIAX"
        "LXFPSKAUTEMNDCMGTSXMXBTUIADNGMGPSRELXNJELX"
        "VRVPRTULHDNQWTWDTYGBPHXTFALJHASVBFXNGLLCHR"
        "ZBWELEKMSJIKNBHWRJGNMGJSGLXFEYPHAGNRBIEQJT"
        "AMRVLCRREMNDGLXRRIMGNSNRWCHRQHAEYEVTAQEBBI"
        "PEEWEVKAKOEWADREMXMTBHHCHRTKDNVRZCHRCLQOHP"
        "WQAIIWXNRMGWOIIFKEE";

    // ÿ���ַ����ĳ���
    int n = ciphertext.length() / 5 + 1;

    // ��ʼ����ά���飬�洢ÿ���и����ַ��ĳ��ִ���
    vector<vector<int>> charCounts(5, vector<int>(26, 0));

    // �������ģ����鲢ͳ���ַ�A-Z���ִ���
    for (int i = 0; i < ciphertext.length(); ++i) {
        int group = i % 5;
        if (isalpha(ciphertext[i])) {
            charCounts[group][ciphertext[i] - 'A']++;
        }
    }

    // ��ʼ��26��Ӣ����ĸ�ĳ���Ƶ��
    float pi[26] = { 0.082, 0.015, 0.028, 0.043, 0.127, 0.022, 0.020, 0.061, 0.070, 0.002,
                     0.008, 0.040, 0.024, 0.067, 0.075, 0.019, 0.001, 0.060, 0.063, 0.091,
                     0.028, 0.010, 0.023, 0.001, 0.020, 0.001 };

    // ���ͳ�ƽ��
    cout << "ͳ�ƽ����" << endl;
    cout << "A �� Z��";
    for (char ch = 'A'; ch <= 'Z'; ++ch) {
        cout << ch << " ";
    }
    cout << endl;
    for (int group = 0; group < 5; ++group) {
        cout << "��" << group + 1 << "�飺";
        for (int i = 0; i < 26; ++i) {
            cout << " " << charCounts[group][i];
        }
        cout << endl;
    }

    // m�������ڴ洢Mg��ֵ
    float m[5][26];

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 26; j++) {
            m[i][j] = 0;  // ��ʼ��Ϊ��
        }

        vector<vector<int>> tempCharCounts(charCounts);  // ����һ���������飬���ڴ洢ÿ����λ�������

        for (int shiftAmount = 0; shiftAmount < 26; shiftAmount++) {
            for (int k = 0; k < 26; k++) {
                charCounts[i][(k + 26 - shiftAmount) % 26] = tempCharCounts[i][k];
            }

            for (int j = 0; j < 26; j++) {
                m[i][shiftAmount] += pi[j] * charCounts[i][j] / n;  // ��������ۼ�
            }
        }
    }

    // ��� m ����
    cout << "\nm ���飺" << endl;
    for (int i = 0; i < 5; i++) {
        cout << "��" << i + 1 << "�飺" << endl;
        for (int j = 0; j < 26; j++) {
            //���ڽ����������λС�����
             cout << " " << fixed << setprecision(3) << round(m[i][j] * 1000) / 1000;
            //cout << " " << m[i][j];
            if (j % 10 == 9) {
                cout << endl;
            }
        }
        cout << endl;
    }

    return 0;
}
