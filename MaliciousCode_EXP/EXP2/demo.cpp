#include <iostream>
#include <fstream>
#include <iomanip>
#include <Windows.h>
#include <locale>
#include <codecvt>
using namespace std;

#pragma pack(push, 1)
struct BPB {
    char unused_data[11];
    uint16_t bytes_per_sector;
    uint8_t sectors_per_cluster;
    uint16_t reserved_sectors;
    uint8_t fat_count;
    uint16_t un_used1;
    uint16_t un_used2;
    uint8_t media_descriptor;
    uint16_t un_used3;
    uint16_t sectors_per_track;
    uint16_t heads;
    uint32_t hidden_sectors;
    uint32_t total_sectors_large;
    char unused_data2[8];
    uint32_t root_directory_cluster_num;
};
#pragma pack(pop)

struct DirectoryEntry {
    char name[8];
    char name_extend[3];
    char unused[9];
    uint16_t high_cluster_num;
    uint16_t creation_time;
    uint16_t creation_date;
    uint16_t cluster_num_low;
    uint32_t file_size;
};



int main() {
    const char* disk = "\\\\.\\E:"; // 修改为你的 U 盘盘符

    ifstream infile(disk, ios::binary);
    if (!infile) {
        cerr << "无法打开设备" << endl;
        return 1;
    }

    BPB bpb;
    infile.seekg(0, ios::beg); // BPB 在 DBR 中的偏移位置
    infile.read(reinterpret_cast<char*>(&bpb), sizeof(BPB));
    cout << "以下数据皆为16进制数\n";
    cout << hex;
    cout << "每扇区字节数: " << bpb.bytes_per_sector << endl;
    cout << "每簇扇区数: " << static_cast<int>(bpb.sectors_per_cluster) << endl;
    cout << "保留扇区数: " << bpb.reserved_sectors << endl;
    cout << "FAT 表个数: " << static_cast<int>(bpb.fat_count) << endl;
    cout << "媒体描述符: 0x" << static_cast<int>(bpb.media_descriptor) << endl;
    cout << "未使用字段1: 0x" << bpb.un_used1 << endl;
    cout << "未使用字段2: 0x" << bpb.un_used2 << endl;
    cout << "未使用字段3: 0x" << bpb.un_used3 << endl;
    cout << "每磁道扇区数: 0x" << bpb.sectors_per_track << endl;
    cout << "磁头数: 0x" << bpb.heads << endl;
    cout << "隐藏扇区数: 0x" << bpb.hidden_sectors << endl;
    cout << "总扇区数 (32 位): 0x" << bpb.total_sectors_large << endl;
    cout << "根目录首簇号: 0x" << bpb.root_directory_cluster_num << endl;

    const int FAT_ENTRY_SIZE = 4; // FAT 表条目大小为 4 个字节
    const int BYTES_PER_SECTOR = 512; // 每个扇区的字节数
    const int MAX_ENTRIES = 15; // 最大输出的目录项数量

    // 计算 FAT 表的起始位置
    uint32_t fat_start_sector = bpb.reserved_sectors;
    uint64_t fat_start_offset = fat_start_sector * BYTES_PER_SECTOR;

    // 移动文件指针到 FAT 表的起始位置
    infile.seekg(fat_start_offset, ios::beg);

    // 读取 FAT 表的内容并输出
    uint32_t fat_entry;
    const int MAX_ENTRIES_FAT = 40;
    int count_fat = 0;
    while (infile.read(reinterpret_cast<char*>(&fat_entry), FAT_ENTRY_SIZE)) {
        // 输出 FAT 表条目的值
        cout << hex << setw(8) << setfill('0') << fat_entry << endl;
        count_fat++;
        // 检测是否遇到了结束标志
        if (count_fat > MAX_ENTRIES_FAT) {
            cout << "遇到了结束标志，停止输出" << endl;
            break;
        }
    }

    // 根据根目录首簇号读取根目录内容表
    uint32_t root_dir_sector = (bpb.root_directory_cluster_num - 2) * bpb.sectors_per_cluster + fat_start_sector;
    uint64_t root_dir_offset = root_dir_sector * BYTES_PER_SECTOR;
    
    // 移动文件指针到 FDT 表的起始位置
    infile.seekg(root_dir_offset, ios::beg);

    // 读取根目录的内容并输出
    DirectoryEntry entry;
    int count = 0; // 目录项计数器
    while (infile.read(reinterpret_cast<char*>(&entry), sizeof(DirectoryEntry))) {
        // 检查是否是一个空目录项（0xE5 表示已删除的文件，0x00 表示未被使用的目录项）
        if (static_cast<uint8_t>(entry.name[0]) == 0x00 || static_cast<uint8_t>(entry.name[0]) == 0xE5) {
            // 未使用的目录项，跳过
            continue;
        }

        // 检查文件名是否为空
        if (entry.name[0] == 0x20) {
            continue;
        }

        // 输出目录项的内容
        cout << "文件名: " << entry.name << "." << entry.name_extend << endl;
        cout << "文件大小: " << entry.file_size << " 字节" << endl;
        // 找到了small.txt的文件首簇号为6，big.txt的文件首簇号为7

        // 输出起始簇号
        uint32_t start_cluster = (static_cast<uint32_t>(entry.high_cluster_num) << 16) | entry.cluster_num_low;
        cout << "文件起始簇号: " << start_cluster << endl;
        cout << endl;

        // 增加计数器
        count++;

        // 如果达到最大输出数量，则退出循环
        if (count >= MAX_ENTRIES) {
            break;
        }
    }
    
    // 将 const char* 类型的字符串转换为 const wchar_t* 类型
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring wdisk = converter.from_bytes(disk);

    // 在 E 盘中查找 small.txt
    std::wstring searchPath = wdisk + L"\\small.txt";
    HANDLE hFile = CreateFile(searchPath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile != INVALID_HANDLE_VALUE) {
        std::cout << "找到 small.txt 的目录项" << std::endl;
        std::cout << "文件大小: " << GetFileSize(hFile, NULL) << " 字节" << std::endl;

        // 读取文件内容
        const DWORD BUFFER_SIZE = 1024;
        char buffer[BUFFER_SIZE];
        DWORD bytesRead;
        if (ReadFile(hFile, buffer, BUFFER_SIZE, &bytesRead, NULL)) {
            std::cout << "small.txt 的数据内容:" << std::endl;
            std::cout.write(buffer, bytesRead) << std::endl;
        }
        CloseHandle(hFile);
    }
    else {
        std::cerr << "未找到 small.txt 的目录项" << std::endl;
    }

    return 0;

}
