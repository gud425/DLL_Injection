// dllmain.cpp : DLL 애플리케이션의 진입점을 정의합니다.
#include "pch.h"
#include<stdio.h>
#include<windows.h>

DWORD WINAPI ThreadProc(LPVOID lparm) {
    int a=0, b=0, c=0;
    int flag = 0,return_val;
    while (1) {
        if (flag)
            break;
        for (int i = 0; i < (a * b * c) + 1; i++) {
            return_val=MessageBox(NULL, L"hacked by nam", L"LOL", MB_YESNOCANCEL|MB_ICONWARNING);
            if (return_val == IDYES)
                a++;
            else if (return_val == IDNO)
                b++;
            else if (return_val == IDCANCEL)
                c++;

            if (a == 4&&b == 2&&c == 5) {
                flag = 1;
                break;
            }
        }

    }
    return 0;
}

BOOL WINAPI DllMain( HMODULE hModule,
                       DWORD  fdwReason,
                       LPVOID lpReserved
                     )
{
    switch (fdwReason)
    {
        HANDLE hThread;
    case DLL_PROCESS_ATTACH:
        hThread = CreateThread(NULL, 0, ThreadProc, NULL, 0, NULL);
        CloseHandle(hThread);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

