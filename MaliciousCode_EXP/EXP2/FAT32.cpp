//#include <iostream>
//#include <windows.h>
//#include <locale>
//#include <codecvt>
//
//int main() {
//    // E �̵ĸ�·��
//    const char* disk = "\\\\.\\E:";
//
//    // �� const char* ���͵��ַ���ת��Ϊ const wchar_t* ����
//    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
//    std::wstring wdisk = converter.from_bytes(disk);
//
//    // �� E ���в��� small.txt
//    std::wstring searchPath = wdisk + L"\\small.txt";
//    HANDLE hFile = CreateFile(searchPath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
//    if (hFile != INVALID_HANDLE_VALUE) {
//        std::cout << "�ҵ� small.txt ��Ŀ¼��" << std::endl;
//        std::cout << "�ļ���С: " << GetFileSize(hFile, NULL) << " �ֽ�" << std::endl;
//
//        // ��ȡ�ļ�����
//        const DWORD BUFFER_SIZE = 1024;
//        char buffer[BUFFER_SIZE];
//        DWORD bytesRead;
//        if (ReadFile(hFile, buffer, BUFFER_SIZE, &bytesRead, NULL)) {
//            std::cout << "small.txt ����������:" << std::endl;
//            std::cout.write(buffer, bytesRead) << std::endl;
//        }
//        CloseHandle(hFile);
//    }
//    else {
//        std::cerr << "δ�ҵ� small.txt ��Ŀ¼��" << std::endl;
//    }
//
//    return 0;
//}
