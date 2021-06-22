#include<iostream>
#include<Windows.h>
#include<TlHelp32.h>
#include <tchar.h>
using namespace std;

void register_program() {
	HKEY  m_hRegsKey;
	const WCHAR* regItemName = L"dll_injection_by_nam";
	const WCHAR* val = L"C:\\Users\\nam\\source\\repos\\dll_injection\\x64\\Debug\\dll_injection.exe";
	RegCreateKey(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", &m_hRegsKey);
	RegSetValueEx(m_hRegsKey, regItemName, NULL, REG_SZ,(const byte*)val, 134);//lstrlen(val)*2
	RegCloseKey(m_hRegsKey);
}
DWORD32 get_pid(wstring proc_name) {

	PROCESSENTRY32 pe32;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	pe32.dwSize = sizeof(PROCESSENTRY32);
	Process32First(hSnapshot, &pe32);
	do {
		wstring num = pe32.szExeFile;
		if (!wcscmp(num.c_str(), proc_name.c_str())) {
			return pe32.th32ProcessID;
		}

	} while (Process32Next(hSnapshot, &pe32));
	return 0;
}
BOOL dll_inject(DWORD hwPID, LPCTSTR DllPath) {
	HANDLE hProcess = NULL;
	HANDLE hThread = NULL; 
	HMODULE hMod = NULL; 
	LPVOID pRemoteBuf = NULL; 
	LPTHREAD_START_ROUTINE pThreadProc; 
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, hwPID);
	
	DWORD dwBufSize = (DWORD)(_tcslen(DllPath) + 1) * sizeof(TCHAR);
	
	pRemoteBuf = VirtualAllocEx(hProcess, NULL, dwBufSize, MEM_COMMIT, PAGE_READWRITE);
	WriteProcessMemory(hProcess, pRemoteBuf, (LPVOID)DllPath, dwBufSize, NULL); 
	hMod = GetModuleHandle(TEXT("kernel32.dll"));
	pThreadProc = (LPTHREAD_START_ROUTINE)GetProcAddress(hMod, "LoadLibraryW");
	hThread = CreateRemoteThread(hProcess, NULL, 0, pThreadProc, pRemoteBuf, 0, NULL);
	WaitForSingleObject(hThread, INFINITE); 
	CloseHandle(hThread); 
	CloseHandle(hProcess); 
	return 1; 
}

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR ﻿lpszCmdParam, int nCmdShow) {
	while (1) {
		register_program();
		DWORD pid_val = get_pid(L"notepad.exe");
		if (pid_val) {
			dll_inject(pid_val,(LPCTSTR)L"C:\\Users\\nam\\source\\repos\\DLL_INJECT\\x64\\Debug\\DLL_INJECT.dll");
		}
		Sleep(500);
	}
}
