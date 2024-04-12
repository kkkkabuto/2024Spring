#include <iostream>
#include <Windows.h>
#include <Imagehlp.h>
#include <fstream>
#include <iomanip>
using namespace std;
#pragma comment(lib, "Imagehlp.lib")


int main() {

	// 1.��ȡָ��������ָ����exe�ļ��ĵ�ַ
    const wchar_t* path = L"E:\\demo2.exe";
	const wchar_t* path2 = L"E:";
	HANDLE hFile;
	HANDLE hMapping;
	LPVOID pMapping;
	hFile = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hFile) {
		return 1;
	}
	//��PE�ļ�ӳ�䵽�ڴ�
	hMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, 0);
	if (!hMapping) {
		return 1;
	}
	pMapping = MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, 0);//���ص���map�Ŀ�ʼ��ַ
	if (!pMapping) {
		return 1;
	}

	PIMAGE_DOS_HEADER dosheader;
	dosheader = (PIMAGE_DOS_HEADER)pMapping;
	if (dosheader->e_magic != IMAGE_DOS_SIGNATURE) {
		cout << "��Ч��PE�ļ�" << endl;
		return 1;
	}

	PIMAGE_NT_HEADERS32 nt_header;
	nt_header = (PIMAGE_NT_HEADERS32)((BYTE*)pMapping + dosheader->e_lfanew);
	if (nt_header->Signature != IMAGE_NT_SIGNATURE) {
		cout << "��Ч��PE�ļ�" << endl;
		return 1;
	}
	// 2.����ͷ�ļ�
	cout << "AddressOfEntryPoint:" << nt_header->OptionalHeader.AddressOfEntryPoint << endl;
	cout << "ImageBase:" << (nt_header->OptionalHeader.ImageBase) << endl;
	cout << "SectionAlignment:" << nt_header->OptionalHeader.SectionAlignment << endl;
	cout << "FileAlignment:" << nt_header->OptionalHeader.FileAlignment << endl;
	cout << "NumberOfSections:" << nt_header->FileHeader.NumberOfSections << endl;

	// 3.�����ڱ�
	PIMAGE_SECTION_HEADER section_header;
	section_header = IMAGE_FIRST_SECTION(nt_header);
	for (int i = 0; i < nt_header->FileHeader.NumberOfSections; i++, section_header++) {
		cout << section_header->Name << "\t" << section_header->Misc.VirtualSize
			<< "\t\t" << section_header->VirtualAddress << "\t\t" << section_header->SizeOfRawData << "\t\t"
			<< section_header->PointerToRawData << "\t\t\t" << section_header->Characteristics << endl;
	}

	// 4.д����������
	DWORD PointerToRawData = IMAGE_FIRST_SECTION(nt_header)->PointerToRawData;
	DWORD SizeOfRawData = IMAGE_FIRST_SECTION(nt_header)->SizeOfRawData;
	UINT8* textContent = new UINT8[SizeOfRawData];
	memcpy(textContent, (UINT8*)pMapping + PointerToRawData, SizeOfRawData);
	
	const wchar_t* disk_path = L"\\\\.\\E:"; // ָ�����̵�·��
	const int sector_size = 512; // ������С���ֽڣ�
	const int sector_number = 4; // ���������ı��

	HANDLE hDisk = CreateFile(disk_path, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	if (hDisk == INVALID_HANDLE_VALUE) {
		std::cerr << "�޷��򿪴��̣�" << disk_path << std::endl;
		return 1;
	}

	// ��������������ƫ����
	DWORD dwMove = SetFilePointer(hDisk, sector_number * sector_size, NULL, FILE_BEGIN);
	if (dwMove == INVALID_SET_FILE_POINTER) {
		std::cerr << "�޷������ļ�ָ�롣�����룺" << GetLastError() << std::endl;
		CloseHandle(hDisk);
		return 1;
	}

	// ������д�뵽��������
	DWORD bytes_written;
	if (!WriteFile(hDisk, textContent, SizeOfRawData, &bytes_written, NULL)) {
		std::cerr << "�޷�д�����ݡ������룺" << GetLastError() << std::endl;
		CloseHandle(hDisk);
		return 1;
	}

	// �رմ��̾��
	CloseHandle(hDisk);

	std::cout << "�����ѳɹ�д�뵽����������" << std::endl;

	// 5.�ڱ�����
	DWORD optionHeaderSize = nt_header->FileHeader.SizeOfOptionalHeader;//��չͷ��С
	DWORD offsetOfFirst = optionHeaderSize + 192;//�ڱ��ͷ��λ��
	
	int numOfSections = nt_header->FileHeader.NumberOfSections;
	DWORD PointerCopy = IMAGE_FIRST_SECTION(nt_header)->PointerToRawData - 40 * numOfSections;//������λ��

	cout << "PointerCopy: " << PointerCopy << endl;
	cout << "offsetOfFirst: " << offsetOfFirst << endl;
	cout << "numOfSections: " << numOfSections << endl;

	
	memcpy((UINT8*)pMapping + PointerCopy, (UINT8*)pMapping + offsetOfFirst, 40 * numOfSections);
	nt_header->FileHeader.SizeOfOptionalHeader += (PointerCopy - offsetOfFirst);

	
	// ���λ���Ƿ���ȷ
	/*cout << nt_header->FileHeader.SizeOfOptionalHeader + (PointerCopy - offsetOfFirst) << endl;
	cout << PointerCopy - offsetOfFirst << endl;*/


	// ��������нڼ����ߣ��޸�VirtualSize��ʹ�����SizeOfRawData
	// �����ڱ���ɽڼ�����
	PIMAGE_SECTION_HEADER current_section = IMAGE_FIRST_SECTION(nt_header);
	for (int i = 0; i < nt_header->FileHeader.NumberOfSections; ++i, ++current_section) {
		// ����ʵ�ʴ�С�������С�Ĳ�ֵ
		DWORD size_difference = current_section->SizeOfRawData - current_section->Misc.VirtualSize;

		// �����ֵ���� 0��˵����Ҫ���нڼ�����
		if (size_difference > 0) {
			// ���ڵ� VirtualSize ����Ϊ SizeOfRawData
			current_section->Misc.VirtualSize = current_section->SizeOfRawData;
		}
	}

	DWORD HeaderCheckSum = nt_header->OptionalHeader.CheckSum;   //PEͷ���У��ֵ
	nt_header->OptionalHeader.CheckSum = 0;

	DWORD CheckSum = 0;
	//����У��ֵ
	MapFileAndCheckSum(path2, &HeaderCheckSum, &CheckSum);
	nt_header->OptionalHeader.CheckSum = CheckSum;//�޸�checkSum

	FlushViewOfFile(pMapping, 0);
	UnmapViewOfFile(pMapping);
	CloseHandle(hMapping);
	CloseHandle(hFile);

    return 0;
}
