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
    const char* disk = "\\\\.\\E:"; // �޸�Ϊ��� U ���̷�

    ifstream infile(disk, ios::binary);
    if (!infile) {
        cerr << "�޷����豸" << endl;
        return 1;
    }

    BPB bpb;
    infile.seekg(0, ios::beg); // BPB �� DBR �е�ƫ��λ��
    infile.read(reinterpret_cast<char*>(&bpb), sizeof(BPB));
    cout << "�������ݽ�Ϊ16������\n";
    cout << hex;
    cout << "ÿ�����ֽ���: " << bpb.bytes_per_sector << endl;
    cout << "ÿ��������: " << static_cast<int>(bpb.sectors_per_cluster) << endl;
    cout << "����������: " << bpb.reserved_sectors << endl;
    cout << "FAT �����: " << static_cast<int>(bpb.fat_count) << endl;
    cout << "ý��������: 0x" << static_cast<int>(bpb.media_descriptor) << endl;
    cout << "δʹ���ֶ�1: 0x" << bpb.un_used1 << endl;
    cout << "δʹ���ֶ�2: 0x" << bpb.un_used2 << endl;
    cout << "δʹ���ֶ�3: 0x" << bpb.un_used3 << endl;
    cout << "ÿ�ŵ�������: 0x" << bpb.sectors_per_track << endl;
    cout << "��ͷ��: 0x" << bpb.heads << endl;
    cout << "����������: 0x" << bpb.hidden_sectors << endl;
    cout << "�������� (32 λ): 0x" << bpb.total_sectors_large << endl;
    cout << "��Ŀ¼�״غ�: 0x" << bpb.root_directory_cluster_num << endl;

    const int FAT_ENTRY_SIZE = 4; // FAT ����Ŀ��СΪ 4 ���ֽ�
    const int BYTES_PER_SECTOR = 512; // ÿ���������ֽ���
    const int MAX_ENTRIES = 15; // ��������Ŀ¼������

    // ���� FAT �����ʼλ��
    uint32_t fat_start_sector = bpb.reserved_sectors;
    uint64_t fat_start_offset = fat_start_sector * BYTES_PER_SECTOR;

    // �ƶ��ļ�ָ�뵽 FAT �����ʼλ��
    infile.seekg(fat_start_offset, ios::beg);

    // ��ȡ FAT ������ݲ����
    uint32_t fat_entry;
    const int MAX_ENTRIES_FAT = 40;
    int count_fat = 0;
    while (infile.read(reinterpret_cast<char*>(&fat_entry), FAT_ENTRY_SIZE)) {
        // ��� FAT ����Ŀ��ֵ
        cout << hex << setw(8) << setfill('0') << fat_entry << endl;
        count_fat++;
        // ����Ƿ������˽�����־
        if (count_fat > MAX_ENTRIES_FAT) {
            cout << "�����˽�����־��ֹͣ���" << endl;
            break;
        }
    }

    // ���ݸ�Ŀ¼�״غŶ�ȡ��Ŀ¼���ݱ�
    uint32_t root_dir_sector = (bpb.root_directory_cluster_num - 2) * bpb.sectors_per_cluster + fat_start_sector;
    uint64_t root_dir_offset = root_dir_sector * BYTES_PER_SECTOR;
    
    // �ƶ��ļ�ָ�뵽 FDT �����ʼλ��
    infile.seekg(root_dir_offset, ios::beg);

    // ��ȡ��Ŀ¼�����ݲ����
    DirectoryEntry entry;
    int count = 0; // Ŀ¼�������
    while (infile.read(reinterpret_cast<char*>(&entry), sizeof(DirectoryEntry))) {
        // ����Ƿ���һ����Ŀ¼�0xE5 ��ʾ��ɾ�����ļ���0x00 ��ʾδ��ʹ�õ�Ŀ¼�
        if (static_cast<uint8_t>(entry.name[0]) == 0x00 || static_cast<uint8_t>(entry.name[0]) == 0xE5) {
            // δʹ�õ�Ŀ¼�����
            continue;
        }

        // ����ļ����Ƿ�Ϊ��
        if (entry.name[0] == 0x20) {
            continue;
        }

        // ���Ŀ¼�������
        cout << "�ļ���: " << entry.name << "." << entry.name_extend << endl;
        cout << "�ļ���С: " << entry.file_size << " �ֽ�" << endl;
        // �ҵ���small.txt���ļ��״غ�Ϊ6��big.txt���ļ��״غ�Ϊ7

        // �����ʼ�غ�
        uint32_t start_cluster = (static_cast<uint32_t>(entry.high_cluster_num) << 16) | entry.cluster_num_low;
        cout << "�ļ���ʼ�غ�: " << start_cluster << endl;
        cout << endl;

        // ���Ӽ�����
        count++;

        // ����ﵽ���������������˳�ѭ��
        if (count >= MAX_ENTRIES) {
            break;
        }
    }
    
    // �� const char* ���͵��ַ���ת��Ϊ const wchar_t* ����
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring wdisk = converter.from_bytes(disk);

    // �� E ���в��� small.txt
    std::wstring searchPath = wdisk + L"\\small.txt";
    HANDLE hFile = CreateFile(searchPath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile != INVALID_HANDLE_VALUE) {
        std::cout << "�ҵ� small.txt ��Ŀ¼��" << std::endl;
        std::cout << "�ļ���С: " << GetFileSize(hFile, NULL) << " �ֽ�" << std::endl;

        // ��ȡ�ļ�����
        const DWORD BUFFER_SIZE = 1024;
        char buffer[BUFFER_SIZE];
        DWORD bytesRead;
        if (ReadFile(hFile, buffer, BUFFER_SIZE, &bytesRead, NULL)) {
            std::cout << "small.txt ����������:" << std::endl;
            std::cout.write(buffer, bytesRead) << std::endl;
        }
        CloseHandle(hFile);
    }
    else {
        std::cerr << "δ�ҵ� small.txt ��Ŀ¼��" << std::endl;
    }

    return 0;

}
