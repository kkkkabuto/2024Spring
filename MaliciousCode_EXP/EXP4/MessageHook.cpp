#include "stdio.h"
#include "conio.h"
#include "windows.h"

#define    DEF_DLL_NAME        "KeyHook.dll"
#define    DEF_HOOKSTART        "HookStart"
#define    DEF_HOOKSTOP        "HookStop"

typedef void (*PFN_HOOKSTART)();
typedef void (*PFN_HOOKSTOP)();

void main()
{
    HMODULE    hDll = NULL;
    PFN_HOOKSTART    HookStart = NULL;
    PFN_HOOKSTOP    HookStop = NULL;
    char    ch = 0;

    // 加载KeyHook.dll
    hDll = LoadLibraryA("D:\\Project\\C++\\KeyHook\\x64\\Debug\\KeyHook.dll");
    if (hDll == NULL)
    {
        printf("LoadLibrary(%s) failed!!! [%d]", DEF_DLL_NAME, GetLastError());
        return;
    }

    // 获取导出函数地址
    HookStart = (PFN_HOOKSTART)GetProcAddress(hDll, DEF_HOOKSTART);
    HookStop = (PFN_HOOKSTOP)GetProcAddress(hDll, DEF_HOOKSTOP);

    // 开始钩取
    HookStart();

    // 等待，直到用户输入“q”
    printf("press 'q' to quit!\n");
    while (_getch() != 'q');

    // 终止钩子
    HookStop();

    // 卸载KeyHook.dll
    FreeLibrary(hDll);
}