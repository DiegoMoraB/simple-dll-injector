#include<windows.h>
#include<iostream>

using namespace std;

int main(int argc,char* argv[]) {// Take paramaters 

	DWORD pid = atoi(argv[1]);// Take the first parameter that will be ProcessID of the programm to inject

	char* DllPath = argv[2]; //Take the second parameter, the dll which will be injected

	DWORD szDll = strlen(DllPath);

	//Declaring variables

	HANDLE hProcess = NULL;
	LPVOID rBuffer = 0x0;
	HMODULE hmodule = NULL;
	LPVOID rProc = 0x0;

	cout << "Getting Handle to the process : " << pid << endl;
	system("pause");

	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

	if (hProcess == NULL) {
		cout << GetLastError() << " error occurred" << endl;
		system("pause");
	}

	rBuffer = VirtualAllocEx(hProcess, NULL, szDll, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);//Allocating mem

	if (rBuffer == NULL) {
		cout << GetLastError() << " error occurred" << endl;
		system("pause");
	}

	if (WriteProcessMemory(hProcess, rBuffer, DllPath, szDll, NULL) == NULL) {
		cout << GetLastError() << " error occurred" << endl;
		system("pause");
	}

	cout << "Mem allocated on " << rBuffer << endl;
	system("pause");

	hmodule = GetModuleHandle(L"kernel32.dll");
	rProc = GetProcAddress(hmodule, "LoadLibraryA");

	CloseHandle(hmodule);

	if (hmodule == NULL || rProc == NULL) {
		cout << GetLastError() << " error occurred" << endl;
		system("pause");
	}

	CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)rProc, rBuffer, 0, NULL);

	CloseHandle(hProcess);

	return 0;
}
