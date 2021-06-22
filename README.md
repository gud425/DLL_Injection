# DLL_Injection
> made by nam


메모장이 켜질때마다 DLL을 삽입하여 내가 원하는 작동을 하도록 실행하는 프로그램    
    

## 소스해석

### 메인소스

```sh
void register_program() {
	HKEY  m_hRegsKey;
	const WCHAR* regItemName = L"dll_injection_by_nam";
	const WCHAR* val = L"C:\\Users\\nam\\source\\repos\\dll_injection\\x64\\Debug\\dll_injection.exe";
	RegCreateKey(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", &m_hRegsKey);
	RegSetValueEx(m_hRegsKey, regItemName, NULL, REG_SZ,(const byte*)val, 134);//lstrlen(val)*2
	RegCloseKey(m_hRegsKey);
}
```
SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run 에 추가하여 컴퓨터 시작될때마다 자동으로 실행되게 만드는 함수 </br></br>


```sh
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
```
구할 프로세스의 이름을 인자로 주면 PID를 리턴하는 함수 </br></br>

```sh
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
```
DLL 인젝션의 핵심 부분으로  </br></br>
