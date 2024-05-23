#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <windows.h>
typedef  void(*PFNHOOKSTART)();
typedef  void(*PFNHOOKSTOP)();
int main()
{
    HMODULE Hmod = LoadLibraryA("D:\\Project\\C++\\SetWindowDLL\\x64\\Debug\\SetWindowDLL.dll");
    PFNHOOKSTART pHookStart = (PFNHOOKSTART)GetProcAddress(Hmod, "HookStart");
    PFNHOOKSTOP pHookStop = (PFNHOOKSTOP)GetProcAddress(Hmod, "HookStop");
    pHookStart();
    printf("print 'q' to quite!\n");
    while (_getch() != 'q');
    pHookStop();
    FreeLibrary(Hmod);
    return 0;
}