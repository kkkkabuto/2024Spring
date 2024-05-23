#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#pragma warning(disable : 4996)
HHOOK ghHook = NULL;
HINSTANCE ghInstance = NULL;
LRESULT CALLBACK KeyboardProc(
    int    code,
    WPARAM wParam,
    LPARAM lParam)
{
    TCHAR szPath[MAX_PATH] = { 0, };
    TCHAR sProcessName[MAX_PATH] = { 0, };
    if (code == 0 && !(lParam & 0x80000000))//������ͷŰ���
    {
        GetModuleFileName(NULL, szPath, MAX_PATH);
        _wsplitpath(szPath, NULL, NULL, sProcessName, NULL);
        if (0 == _wcsicmp(sProcessName, L"notepad"))//�����������notepad
        {
            return 1;//ɾ����Ϣ���������´���
        }
    }
    return CallNextHookEx(ghHook, code, wParam, lParam);//����������Ϣ
}

BOOL APIENTRY DllMain(
    HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        ghInstance = hModule;//��ñ�ʵ����ģ����
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

extern "C"
{
    __declspec(dllexport) void HookStart()
    {
        ghHook = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, ghInstance, 0);
    }
    __declspec(dllexport) void HookStop()
    {
        if (ghHook)
        {
            UnhookWindowsHookEx(ghHook);
            ghHook = NULL;
        }
    }
}