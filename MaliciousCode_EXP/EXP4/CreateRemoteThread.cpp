#include "windows.h"
#include "stdio.h"
#include "tlhelp32.h"
#include "io.h"
#include "tchar.h"

//判断某模块(dll)是否在相应的进程中
//dwPID         进程的PID
//szDllPath     查询的dll的完整路径
BOOL CheckDllInProcess(DWORD dwPID, LPCTSTR szDllPath)
{
    BOOL                    bMore = FALSE;
    HANDLE                  hSnapshot = INVALID_HANDLE_VALUE;
    MODULEENTRY32           me = { sizeof(me), };

    if (INVALID_HANDLE_VALUE ==
        (hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID)))//获得进程的快照
    {
        _tprintf(L"CheckDllInProcess() : CreateToolhelp32Snapshot(%d) failed!!! [%d]\n",
            dwPID, GetLastError());
        return FALSE;
    }
    bMore = Module32First(hSnapshot, &me);//遍历进程内得的所有模块
    for (; bMore; bMore = Module32Next(hSnapshot, &me))
    {
        if (!_tcsicmp(me.szModule, szDllPath) || !_tcsicmp(me.szExePath, szDllPath))//模块名或含路径的名相符
        {
            CloseHandle(hSnapshot);
            return TRUE;
        }
    }
    CloseHandle(hSnapshot);
    return FALSE;
}

//向指定的进程注入相应的模块
//dwPID         目标进程的PID
//szDllPath     被注入的dll的完整路径
BOOL InjectDll(DWORD dwPID, LPCTSTR szDllPath)
{
    HANDLE   hProcess = NULL;//保存目标进程的句柄
    LPVOID    pRemoteBuf = NULL;//目标进程开辟的内存的起始地址
    DWORD   dwBufSize = (DWORD)(_tcslen(szDllPath) + 1) * sizeof(TCHAR);//开辟的内存的大小
    LPTHREAD_START_ROUTINE  pThreadProc = NULL;//loadLibreayW函数的起始地址
    HMODULE   hMod = NULL;//kernel32.dll模块的句柄
    BOOL        bRet = FALSE;
    if (!(hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID)))//打开目标进程，获得句柄
    {
        _tprintf(L"InjectDll() : OpenProcess(%d) failed!!! [%d]\n",
            dwPID, GetLastError());
        goto INJECTDLL_EXIT;
    }
    pRemoteBuf = VirtualAllocEx(hProcess, NULL, dwBufSize,
        MEM_COMMIT, PAGE_READWRITE);//在目标进程空间开辟一块内存
    if (pRemoteBuf == NULL)
    {
        _tprintf(L"InjectDll() : VirtualAllocEx() failed!!! [%d]\n",
            GetLastError());
        goto INJECTDLL_EXIT;
    }
    if (!WriteProcessMemory(hProcess, pRemoteBuf,
        (LPVOID)szDllPath, dwBufSize, NULL))//向开辟的内存复制dll的路径
    {
        _tprintf(L"InjectDll() : WriteProcessMemory() failed!!! [%d]\n",
            GetLastError());
        goto INJECTDLL_EXIT;
    }
    hMod = GetModuleHandle(L"kernel32.dll");//获得本进程kernel32.dll的模块句柄
    if (hMod == NULL)
    {
        _tprintf(L"InjectDll() : GetModuleHandle(\"kernel32.dll\") failed!!! [%d]\n",
            GetLastError());
        goto INJECTDLL_EXIT;
    }
    pThreadProc = (LPTHREAD_START_ROUTINE)GetProcAddress(hMod, "LoadLibraryW");//获得LoadLibraryW函数的起始地址
    if (pThreadProc == NULL)
    {
        _tprintf(L"InjectDll() : GetProcAddress(\"LoadLibraryW\") failed!!! [%d]\n",
            GetLastError());
        goto INJECTDLL_EXIT;
    }
    if (!CreateRemoteThread(hProcess, NULL, 0, pThreadProc, pRemoteBuf, 0, NULL))//执行远程线程
    {
        _tprintf(L"InjectDll() : MyCreateRemoteThread() failed!!!\n");
        goto INJECTDLL_EXIT;
    }
INJECTDLL_EXIT:
    bRet = CheckDllInProcess(dwPID, szDllPath);//确认结果
    if (pRemoteBuf)
        VirtualFreeEx(hProcess, pRemoteBuf, 0, MEM_RELEASE);
    if (hProcess)
        CloseHandle(hProcess);
    return bRet;
}

//让指定的进程卸载相应的模块
//dwPID         目标进程的PID
//szDllPath     被注入的dll的完整路径,注意：路径不要用“/”来代替“\\”
BOOL EjectDll(DWORD dwPID, LPCTSTR szDllPath)
{
    BOOL     bMore = FALSE, bFound = FALSE, bRet = FALSE;
    HANDLE   hSnapshot = INVALID_HANDLE_VALUE;
    HANDLE   hProcess = NULL;
    MODULEENTRY32  me = { sizeof(me), };
    LPTHREAD_START_ROUTINE  pThreadProc = NULL;
    HMODULE  hMod = NULL;
    TCHAR     szProcName[MAX_PATH] = { 0, };
    if (INVALID_HANDLE_VALUE == (hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID)))
    {
        _tprintf(L"EjectDll() : CreateToolhelp32Snapshot(%d) failed!!! [%d]\n",
            dwPID, GetLastError());
        goto EJECTDLL_EXIT;
    }
    bMore = Module32First(hSnapshot, &me);
    for (; bMore; bMore = Module32Next(hSnapshot, &me))//查找模块句柄
    {
        if (!_tcsicmp(me.szModule, szDllPath) ||
            !_tcsicmp(me.szExePath, szDllPath))
        {
            bFound = TRUE;
            break;
        }
    }
    if (!bFound)
    {
        _tprintf(L"EjectDll() : There is not %s module in process(%d) memory!!!\n",
            szDllPath, dwPID);
        goto EJECTDLL_EXIT;
    }
    if (!(hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID)))
    {
        _tprintf(L"EjectDll() : OpenProcess(%d) failed!!! [%d]\n",
            dwPID, GetLastError());
        goto EJECTDLL_EXIT;
    }
    hMod = GetModuleHandle(L"kernel32.dll");
    if (hMod == NULL)
    {
        _tprintf(L"EjectDll() : GetModuleHandle(\"kernel32.dll\") failed!!! [%d]\n",
            GetLastError());
        goto EJECTDLL_EXIT;
    }
    pThreadProc = (LPTHREAD_START_ROUTINE)GetProcAddress(hMod, "FreeLibrary");
    if (pThreadProc == NULL)
    {
        _tprintf(L"EjectDll() : GetProcAddress(\"FreeLibrary\") failed!!! [%d]\n",
            GetLastError());
        goto EJECTDLL_EXIT;
    }
    if (!CreateRemoteThread(hProcess, NULL, 0, pThreadProc, me.modBaseAddr, 0, NULL))
    {
        _tprintf(L"EjectDll() : MyCreateRemoteThread() failed!!!\n");
        goto EJECTDLL_EXIT;
    }
    bRet = TRUE;
EJECTDLL_EXIT:
    if (hProcess)
        CloseHandle(hProcess);
    if (hSnapshot != INVALID_HANDLE_VALUE)
        CloseHandle(hSnapshot);
    return bRet;
}

int main()
{
    //InjectDll(3400, L"D:\\Project\\C++\\KeyHook\\x64\\Debug\\KeyHook.dll");
    EjectDll(3400, L"D:\\Project\\C++\\KeyHook\\x64\\Debug\\KeyHook.dll");
    return 0;
}