#include "windows.h"
#include "stdio.h"
#include "tlhelp32.h"
#include "io.h"
#include "tchar.h"

//�ж�ĳģ��(dll)�Ƿ�����Ӧ�Ľ�����
//dwPID         ���̵�PID
//szDllPath     ��ѯ��dll������·��
BOOL CheckDllInProcess(DWORD dwPID, LPCTSTR szDllPath)
{
    BOOL                    bMore = FALSE;
    HANDLE                  hSnapshot = INVALID_HANDLE_VALUE;
    MODULEENTRY32           me = { sizeof(me), };

    if (INVALID_HANDLE_VALUE ==
        (hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID)))//��ý��̵Ŀ���
    {
        _tprintf(L"CheckDllInProcess() : CreateToolhelp32Snapshot(%d) failed!!! [%d]\n",
            dwPID, GetLastError());
        return FALSE;
    }
    bMore = Module32First(hSnapshot, &me);//���������ڵõ�����ģ��
    for (; bMore; bMore = Module32Next(hSnapshot, &me))
    {
        if (!_tcsicmp(me.szModule, szDllPath) || !_tcsicmp(me.szExePath, szDllPath))//ģ������·���������
        {
            CloseHandle(hSnapshot);
            return TRUE;
        }
    }
    CloseHandle(hSnapshot);
    return FALSE;
}

//��ָ���Ľ���ע����Ӧ��ģ��
//dwPID         Ŀ����̵�PID
//szDllPath     ��ע���dll������·��
BOOL InjectDll(DWORD dwPID, LPCTSTR szDllPath)
{
    HANDLE   hProcess = NULL;//����Ŀ����̵ľ��
    LPVOID    pRemoteBuf = NULL;//Ŀ����̿��ٵ��ڴ����ʼ��ַ
    DWORD   dwBufSize = (DWORD)(_tcslen(szDllPath) + 1) * sizeof(TCHAR);//���ٵ��ڴ�Ĵ�С
    LPTHREAD_START_ROUTINE  pThreadProc = NULL;//loadLibreayW��������ʼ��ַ
    HMODULE   hMod = NULL;//kernel32.dllģ��ľ��
    BOOL        bRet = FALSE;
    if (!(hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID)))//��Ŀ����̣���þ��
    {
        _tprintf(L"InjectDll() : OpenProcess(%d) failed!!! [%d]\n",
            dwPID, GetLastError());
        goto INJECTDLL_EXIT;
    }
    pRemoteBuf = VirtualAllocEx(hProcess, NULL, dwBufSize,
        MEM_COMMIT, PAGE_READWRITE);//��Ŀ����̿ռ俪��һ���ڴ�
    if (pRemoteBuf == NULL)
    {
        _tprintf(L"InjectDll() : VirtualAllocEx() failed!!! [%d]\n",
            GetLastError());
        goto INJECTDLL_EXIT;
    }
    if (!WriteProcessMemory(hProcess, pRemoteBuf,
        (LPVOID)szDllPath, dwBufSize, NULL))//�򿪱ٵ��ڴ渴��dll��·��
    {
        _tprintf(L"InjectDll() : WriteProcessMemory() failed!!! [%d]\n",
            GetLastError());
        goto INJECTDLL_EXIT;
    }
    hMod = GetModuleHandle(L"kernel32.dll");//��ñ�����kernel32.dll��ģ����
    if (hMod == NULL)
    {
        _tprintf(L"InjectDll() : GetModuleHandle(\"kernel32.dll\") failed!!! [%d]\n",
            GetLastError());
        goto INJECTDLL_EXIT;
    }
    pThreadProc = (LPTHREAD_START_ROUTINE)GetProcAddress(hMod, "LoadLibraryW");//���LoadLibraryW��������ʼ��ַ
    if (pThreadProc == NULL)
    {
        _tprintf(L"InjectDll() : GetProcAddress(\"LoadLibraryW\") failed!!! [%d]\n",
            GetLastError());
        goto INJECTDLL_EXIT;
    }
    if (!CreateRemoteThread(hProcess, NULL, 0, pThreadProc, pRemoteBuf, 0, NULL))//ִ��Զ���߳�
    {
        _tprintf(L"InjectDll() : MyCreateRemoteThread() failed!!!\n");
        goto INJECTDLL_EXIT;
    }
INJECTDLL_EXIT:
    bRet = CheckDllInProcess(dwPID, szDllPath);//ȷ�Ͻ��
    if (pRemoteBuf)
        VirtualFreeEx(hProcess, pRemoteBuf, 0, MEM_RELEASE);
    if (hProcess)
        CloseHandle(hProcess);
    return bRet;
}

//��ָ���Ľ���ж����Ӧ��ģ��
//dwPID         Ŀ����̵�PID
//szDllPath     ��ע���dll������·��,ע�⣺·����Ҫ�á�/�������桰\\��
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
    for (; bMore; bMore = Module32Next(hSnapshot, &me))//����ģ����
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