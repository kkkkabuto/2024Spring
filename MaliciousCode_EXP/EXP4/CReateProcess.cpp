#include <windows.h>
#include <stdio.h>
#pragma warning(disable : 4996) 

//���ӽ��̴�������ʱע��dll
//hProcess      ������ʱ����Ľ��̾��
//hThread       �����б�������߳̾��
//szDllPath     ��ע���dll������·��
BOOL StartHook(HANDLE hProcess, HANDLE hThread, TCHAR* szDllPath)
{
    BYTE ShellCode[30 + MAX_PATH * sizeof(TCHAR)] =
    {
        0x60,               //pushad
        0x9c,               //pushfd
        0x68,0xaa,0xbb,0xcc,0xdd,   //push xxxxxxxx(xxxxxxxx��ƫ��Ϊ3)
        0xff,0x15,0xdd,0xcc,0xbb,0xaa,  //call [addr]([addr]��ƫ��Ϊ9)
        0x9d,               //popfd
        0x61,               //popad
        0xff,0x25,0xaa,0xbb,0xcc,0xdd,  //jmp [Eip]([Eip]��ƫ��Ϊ17)
        0xaa,0xaa,0xaa,0xaa,        //����loadlibraryW�����ĵ�ַ(ƫ��Ϊ21)
        0xaa,0xaa,0xaa,0xaa,        //���洴������ʱ��������߳�Eip(ƫ��Ϊ25)
        0,              //����dll·���ַ���(ƫ��Ϊ29)
    };
    CONTEXT ctx;
    ctx.ContextFlags = CONTEXT_ALL;
    if (!GetThreadContext(hThread, &ctx))
    {
        printf("GetThreadContext() ErrorCode:[0x%08x]\n", GetLastError());
        return FALSE;
    }
    //��Ŀ������ڴ�ռ����һ���ִ�е��ڴ�
    LPVOID LpAddr = VirtualAllocEx(hProcess, NULL, 30 + MAX_PATH * sizeof(TCHAR), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    if (LpAddr == NULL)
    {
        printf("VirtualAllocEx() ErrorCode:[0x%08x]\n", GetLastError());
        return FALSE;
    }
    //���LoadLibraryW�����ĵ�ַ
    DWORD LoadDllAAddr = (DWORD)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryW");
    if (LoadDllAAddr == NULL)
    {
        printf("GetProcAddress() ErrorCode:[0x%08x]\n", GetLastError());
        return FALSE;
    }
    printf("ԭʼEip=0x%08x\n", ctx.Eip);
    //д��dllpath
    memcpy((char*)(ShellCode + 29), szDllPath, MAX_PATH);
    //д��push xxxxxxxx
    *(DWORD*)(ShellCode + 3) = (DWORD)LpAddr + 29;
    //д��loadlibraryA��ַ
    *(DWORD*)(ShellCode + 21) = LoadDllAAddr;
    //д��call [addr]��[addr]
    *(DWORD*)(ShellCode + 9) = (DWORD)LpAddr + 21;
    //д��ԭʼEip
    *(DWORD*)(ShellCode + 25) = ctx.Eip;
    //д��jmp [Eip]��[Eip]
    *(DWORD*)(ShellCode + 17) = (DWORD)LpAddr + 25;
    //��shellcodeд��Ŀ�����
    if (!WriteProcessMemory(hProcess, LpAddr, ShellCode, 30 + MAX_PATH * sizeof(TCHAR), NULL))
    {
        printf("WriteProcessMemory() ErrorCode:[0x%08x]\n", GetLastError());
        return FALSE;
    }
    //�޸�Ŀ����̵�Eip��ִ�б�ע��Ĵ���
    ctx.Eip = (DWORD)LpAddr;
    if (!SetThreadContext(hThread, &ctx))
    {
        printf("SetThreadContext() ErrorCode:[0x%08x]\n", GetLastError());
        return FALSE;
    }
    printf("�޸ĺ�Eip=0x%08x\n", ctx.Eip);
    return TRUE;
};

int main()
{
    STARTUPINFO sti;
    PROCESS_INFORMATION proci;
    memset(&sti, 0, sizeof(STARTUPINFO));
    memset(&proci, 0, sizeof(PROCESS_INFORMATION));
    sti.cb = sizeof(STARTUPINFO);
    wchar_t ExeName[MAX_PATH] = L"D:\\StudyFile\\2024Spring\\�������\\ʵ����\\main.exe";//�ӽ��̵����ּ���������
    wchar_t DllName[MAX_PATH] = L"C:\\Windows\\SysWOW64\\crtdll.dll";//��ע���dll������·��
    if (CreateProcess(NULL, ExeName, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &sti, &proci) == NULL)
    {
        printf("CreateProcess() ErrorCode:[0x%08x]\n", GetLastError());
        getchar();
        return 0;
    }
    if (!StartHook(proci.hProcess, proci.hThread, DllName))
    {
        TerminateProcess(proci.hProcess, 0);
        printf("Terminated Process\n");
        getchar();
        return 0;
    }
    ResumeThread(proci.hThread);
    CloseHandle(proci.hProcess);
    CloseHandle(proci.hThread);
    return 0;
}