#include <iostream>
#include <windows.h>
#include <cstddef>
template <typename T>
T rva_to_va(void* base, ptrdiff_t rva) {
    static_assert(std::is_pointer<T>::value, "rva_to_va return type must be a pointer.");
    return reinterpret_cast<T>(static_cast<uint8_t*>(base) + rva);
}

// ���ڱ���ԭʼ�����ĵ�ַ
using sleep_t = void* (WINAPI*)(DWORD);
sleep_t original_sleep;

void WINAPI my_sleep(DWORD ms) {
    std::cout << "[?] Hooked Sleep Function Called!" << std::endl;
    std::cout << "Sleeping for: 0x" << ms << std::endl;
    // ����ԭ����
    original_sleep(ms);
}

bool hook_iat(const char* module_name, const char* function_name, void* hook_func, void** original_func) {
    // ��ȡDOSͷָ��
    auto dos_header = reinterpret_cast<IMAGE_DOS_HEADER*>(GetModuleHandle(nullptr));
    // ��ȡNTͷָ��
    auto nt_header = rva_to_va<IMAGE_NT_HEADERS*>(dos_header, dos_header->e_lfanew);
    // ���У���֤��ȡ������Ч��NTͷָ��
    if (nt_header->Signature != IMAGE_NT_SIGNATURE)
        return false;

    // ��ȡ����������ָ��
    auto import_descriptors = rva_to_va<IMAGE_IMPORT_DESCRIPTOR*>(dos_header,
        nt_header->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

    // ѭ�����������������е���Ŀ
    for (size_t i = 0; import_descriptors[i].Characteristics != 0; i++) {
        auto dll_name = rva_to_va<char*>(dos_header, import_descriptors[i].Name);
        // �����������Ҫ�ҵ�ģ�飬������
        if (_strcmpi(dll_name, module_name) != 0)
            continue;

        if (!import_descriptors[i].FirstThunk || !import_descriptors[i].OriginalFirstThunk)
            return false;

        auto thunk = rva_to_va<IMAGE_THUNK_DATA*>(dos_header, import_descriptors[i].FirstThunk);
        auto original_thunk = rva_to_va<IMAGE_THUNK_DATA*>(dos_header, import_descriptors[i].OriginalFirstThunk);
        for (; original_thunk->u1.Function; original_thunk++, thunk++) {
            // ����ͨ�����������
            if (original_thunk->u1.Ordinal & IMAGE_ORDINAL_FLAG)
                continue;

            auto import = rva_to_va<IMAGE_IMPORT_BY_NAME*>(dos_header, original_thunk->u1.AddressOfData);
            // �����������Ҫ�ҵĺ�����������
            if (_strcmpi(function_name, import->Name) != 0)
                continue;

            // �ҵ�������ҪHook�ĺ���
            // �Ƚ��ڴ汣����Ϊ��д��
            MEMORY_BASIC_INFORMATION mbi;
            VirtualQuery(thunk, &mbi, sizeof(mbi));
            if (!VirtualProtect(mbi.BaseAddress, mbi.RegionSize, PAGE_EXECUTE_READWRITE, &mbi.Protect))
                return false;

            // ����ԭ������ַ
            *original_func = reinterpret_cast<void**>(thunk->u1.Function);
            // ��Hook������ַ������ȥ
            thunk->u1.Function = reinterpret_cast<uintptr_t>(hook_func);
            // �ָ��ڴ汣��
            DWORD _;
            if (VirtualProtect(mbi.BaseAddress, mbi.RegionSize, mbi.Protect, &_))
                return true;
        }
    }

    return false;
}

int main() {

    

    std::cout << "test" << std::endl;
    void* original_func;

    std::cout << "IAT Hook Example by YangJiajun\n" << std::endl;
    
    if (!hook_iat("kernel32.dll", "Sleep", my_sleep, &original_func)) {

        std::cout << "[-] Hooking failed! error: " << GetLastError() << std::endl;
    }
    else {
        std::cout << std::hex <<
            "[?] Old Address: 0x" << original_func << std::endl <<
            "[+] New Address: 0x" << my_sleep << std::endl;
        original_sleep = static_cast<sleep_t>(original_func);
        Sleep(0x10000);
    }

    return 0;
}