#include <iostream>
#include <Windows.h>
#include <Imagehlp.h>
#include <fstream>
#include <iomanip>
using namespace std;
#pragma comment(lib, "Imagehlp.lib")


int main() {

	// 1.获取指定磁盘中指定的exe文件的地址
    const wchar_t* path = L"E:\\demo2.exe";
	const wchar_t* path2 = L"E:";
	HANDLE hFile;
	HANDLE hMapping;
	LPVOID pMapping;
	hFile = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hFile) {
		return 1;
	}
	//将PE文件映射到内存
	hMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, 0);
	if (!hMapping) {
		return 1;
	}
	pMapping = MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, 0);//返回的是map的开始地址
	if (!pMapping) {
		return 1;
	}

	PIMAGE_DOS_HEADER dosheader;
	dosheader = (PIMAGE_DOS_HEADER)pMapping;
	if (dosheader->e_magic != IMAGE_DOS_SIGNATURE) {
		cout << "无效的PE文件" << endl;
		return 1;
	}

	PIMAGE_NT_HEADERS32 nt_header;
	nt_header = (PIMAGE_NT_HEADERS32)((BYTE*)pMapping + dosheader->e_lfanew);
	if (nt_header->Signature != IMAGE_NT_SIGNATURE) {
		cout << "无效的PE文件" << endl;
		return 1;
	}
	// 2.解析头文件
	cout << "AddressOfEntryPoint:" << nt_header->OptionalHeader.AddressOfEntryPoint << endl;
	cout << "ImageBase:" << (nt_header->OptionalHeader.ImageBase) << endl;
	cout << "SectionAlignment:" << nt_header->OptionalHeader.SectionAlignment << endl;
	cout << "FileAlignment:" << nt_header->OptionalHeader.FileAlignment << endl;
	cout << "NumberOfSections:" << nt_header->FileHeader.NumberOfSections << endl;

	// 3.解析节表
	PIMAGE_SECTION_HEADER section_header;
	section_header = IMAGE_FIRST_SECTION(nt_header);
	for (int i = 0; i < nt_header->FileHeader.NumberOfSections; i++, section_header++) {
		cout << section_header->Name << "\t" << section_header->Misc.VirtualSize
			<< "\t\t" << section_header->VirtualAddress << "\t\t" << section_header->SizeOfRawData << "\t\t"
			<< section_header->PointerToRawData << "\t\t\t" << section_header->Characteristics << endl;
	}

	// 4.写到隐藏扇区
	DWORD PointerToRawData = IMAGE_FIRST_SECTION(nt_header)->PointerToRawData;
	DWORD SizeOfRawData = IMAGE_FIRST_SECTION(nt_header)->SizeOfRawData;
	UINT8* textContent = new UINT8[SizeOfRawData];
	memcpy(textContent, (UINT8*)pMapping + PointerToRawData, SizeOfRawData);
	
	const wchar_t* disk_path = L"\\\\.\\E:"; // 指定磁盘的路径
	const int sector_size = 512; // 扇区大小（字节）
	const int sector_number = 4; // 隐藏扇区的编号

	HANDLE hDisk = CreateFile(disk_path, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	if (hDisk == INVALID_HANDLE_VALUE) {
		std::cerr << "无法打开磁盘：" << disk_path << std::endl;
		return 1;
	}

	// 计算隐藏扇区的偏移量
	DWORD dwMove = SetFilePointer(hDisk, sector_number * sector_size, NULL, FILE_BEGIN);
	if (dwMove == INVALID_SET_FILE_POINTER) {
		std::cerr << "无法设置文件指针。错误码：" << GetLastError() << std::endl;
		CloseHandle(hDisk);
		return 1;
	}

	// 将数据写入到隐藏扇区
	DWORD bytes_written;
	if (!WriteFile(hDisk, textContent, SizeOfRawData, &bytes_written, NULL)) {
		std::cerr << "无法写入数据。错误码：" << GetLastError() << std::endl;
		CloseHandle(hDisk);
		return 1;
	}

	// 关闭磁盘句柄
	CloseHandle(hDisk);

	std::cout << "数据已成功写入到隐藏扇区。" << std::endl;

	// 5.节表免疫
	DWORD optionHeaderSize = nt_header->FileHeader.SizeOfOptionalHeader;//扩展头大小
	DWORD offsetOfFirst = optionHeaderSize + 192;//节表项开头的位置
	
	int numOfSections = nt_header->FileHeader.NumberOfSections;
	DWORD PointerCopy = IMAGE_FIRST_SECTION(nt_header)->PointerToRawData - 40 * numOfSections;//拷贝的位置

	cout << "PointerCopy: " << PointerCopy << endl;
	cout << "offsetOfFirst: " << offsetOfFirst << endl;
	cout << "numOfSections: " << numOfSections << endl;

	
	memcpy((UINT8*)pMapping + PointerCopy, (UINT8*)pMapping + offsetOfFirst, 40 * numOfSections);
	nt_header->FileHeader.SizeOfOptionalHeader += (PointerCopy - offsetOfFirst);

	
	// 检查位置是否正确
	/*cout << nt_header->FileHeader.SizeOfOptionalHeader + (PointerCopy - offsetOfFirst) << endl;
	cout << PointerCopy - offsetOfFirst << endl;*/


	// 在这里进行节间免疫，修改VirtualSize，使其等于SizeOfRawData
	// 遍历节表，完成节间免疫
	PIMAGE_SECTION_HEADER current_section = IMAGE_FIRST_SECTION(nt_header);
	for (int i = 0; i < nt_header->FileHeader.NumberOfSections; ++i, ++current_section) {
		// 计算实际大小和虚拟大小的差值
		DWORD size_difference = current_section->SizeOfRawData - current_section->Misc.VirtualSize;

		// 如果差值大于 0，说明需要进行节间免疫
		if (size_difference > 0) {
			// 将节的 VirtualSize 设置为 SizeOfRawData
			current_section->Misc.VirtualSize = current_section->SizeOfRawData;
		}
	}

	DWORD HeaderCheckSum = nt_header->OptionalHeader.CheckSum;   //PE头里的校验值
	nt_header->OptionalHeader.CheckSum = 0;

	DWORD CheckSum = 0;
	//计算校验值
	MapFileAndCheckSum(path2, &HeaderCheckSum, &CheckSum);
	nt_header->OptionalHeader.CheckSum = CheckSum;//修改checkSum

	FlushViewOfFile(pMapping, 0);
	UnmapViewOfFile(pMapping);
	CloseHandle(hMapping);
	CloseHandle(hFile);

    return 0;
}
