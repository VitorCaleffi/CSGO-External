#include "Memory.h"

namespace Memory
{
	struct Module_t
	{
		DWORD		dwBase;
		DWORD		dwSize;
		std::string	szName;

	}Module[MAX_MEMORY_MODULE];

	DWORD	ProcessID = 0x0;
	HANDLE	hProcess = NULL;
	HWND	hWindow = NULL;
	int		iLastAddedModule = 0;
	int GetLastAddedModule(void)
	{
		return iLastAddedModule;
	}
	
	bool CompareBytes(const unsigned char* bytes, const char* pattern)
	{
		for (; *pattern; *pattern != ' ' ? ++bytes : bytes, ++pattern) {
			if (*pattern == ' ' || *pattern == '?')
				continue;
			if (*bytes != getByte(pattern))
				return false;
			++pattern;
		}
		return true;
	}

	uintptr_t FindPattern(const std::string& module, const char* pattern, uintptr_t patternOffset, uintptr_t addressOffset)
	{
		auto ImgBase = GetModule(module);
		auto ImgSize = GetModuleSize(module);

		BYTE* Data = new BYTE[ImgSize];
		unsigned long bytesRead;

		ReadProcessMemory(hProcess, (LPVOID)ImgBase, Data, ImgSize, &bytesRead);

		auto pb = const_cast< unsigned char* >(Data);
		auto max = ImgBase - 0x1000;

		for (auto off = 0UL; off < max; ++off) {
			if (CompareBytes(pb + off, pattern)) {

				auto add = ImgBase + off + patternOffset;

				ReadProcessMemory(hProcess, (LPVOID)add, &add, sizeof(uintptr_t), &bytesRead);
				add -= ImgBase;

				free(Data);
				return add + addressOffset;
				
			}
		}
		free(Data);
		return 0;
	}

	DWORD GetModuleSize(std::string szModuleName)
	{
		for (int i = 0; i < GetLastAddedModule(); i++)
		{
			if (!strcmp((char*)szModuleName.c_str(), (char*)Module[i].szName.c_str()))
				return Module[i].dwSize;
		}
		return 0x0;
	}

	MODULEENTRY32 GetModuleBaseExtern(std::string szModuleName)
	{
		MODULEENTRY32	ME32 = { 0 };
		HANDLE			hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, ProcessID);

		if (!hSnapshot)
			return ME32;

		ME32.dwSize = sizeof(ME32);

		BOOL			bModule = Module32First(hSnapshot, &ME32);

		while (bModule)
		{
			if (!strcmp((char*)ME32.szModule, (char*)szModuleName.c_str()))
			{
				CloseHandle(hSnapshot);
				return ME32;
			}
			bModule = Module32Next(hSnapshot, &ME32);
		}
		CloseHandle(hSnapshot);
		return ME32;
	}
	void SetLastAddedModule(int Value)
	{
		iLastAddedModule = Value;
	}
	bool Initialize(std::string szGameWindowTitle)
	{
		if (!(char*)szGameWindowTitle.c_str())
			return false;

		while (!hWindow)
		{
			hWindow = FindWindowA(NULL, (char*)szGameWindowTitle.c_str());
			Sleep(5);
		}

		GetWindowThreadProcessId(hWindow, &ProcessID);
		if (!ProcessID)
			return false;

		hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, ProcessID);
		if (!hProcess)
			return false;
		return true;
	}

	DWORD GetModule(std::string szName)
	{
		for (int i = 0; i < GetLastAddedModule(); i++)
		{
			if (!strcmp((char*)szName.c_str(), (char*)Module[i].szName.c_str()))
				return Module[i].dwBase;
		}
		return 0x0;
	}


	void AddModule(std::string szName, std::string szModuleName)
	{
		int i = GetLastAddedModule();

		if (i > MAX_MEMORY_MODULE - 1)
			return;

		Module[i].dwBase = 0x0;
		Module[i].szName = szName;

		while (Module[i].dwBase == 0x0)
		{
			MODULEENTRY32 ModBuff = GetModuleBaseExtern(szModuleName);
			Module[i].dwBase = (DWORD)ModBuff.modBaseAddr;
			Module[i].dwSize = (DWORD)ModBuff.modBaseSize;
			Sleep(5);
		}

		SetLastAddedModule(i + 1);
	}
}