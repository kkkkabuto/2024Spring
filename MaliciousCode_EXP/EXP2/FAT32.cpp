//#include <iostream>
//#include <windows.h>
//#include <locale>
//#include <codecvt>
//
//int main() {
//    // E 盘的根路径
//    const char* disk = "\\\\.\\E:";
//
//    // 将 const char* 类型的字符串转换为 const wchar_t* 类型
//    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
//    std::wstring wdisk = converter.from_bytes(disk);
//
//    // 在 E 盘中查找 small.txt
//    std::wstring searchPath = wdisk + L"\\small.txt";
//    HANDLE hFile = CreateFile(searchPath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
//    if (hFile != INVALID_HANDLE_VALUE) {
//        std::cout << "找到 small.txt 的目录项" << std::endl;
//        std::cout << "文件大小: " << GetFileSize(hFile, NULL) << " 字节" << std::endl;
//
//        // 读取文件内容
//        const DWORD BUFFER_SIZE = 1024;
//        char buffer[BUFFER_SIZE];
//        DWORD bytesRead;
//        if (ReadFile(hFile, buffer, BUFFER_SIZE, &bytesRead, NULL)) {
//            std::cout << "small.txt 的数据内容:" << std::endl;
//            std::cout.write(buffer, bytesRead) << std::endl;
//        }
//        CloseHandle(hFile);
//    }
//    else {
//        std::cerr << "未找到 small.txt 的目录项" << std::endl;
//    }
//
//    return 0;
//}
