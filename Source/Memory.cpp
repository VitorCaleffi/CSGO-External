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
	/*
	DWORD FindPattern(BYTE * ArrayOfBytes, std::string Module, int SizeOf)
	{
		long int x = 0, y = 0;

		DWORD ModuleSize = Memory::GetModuleSize(Module);
		DWORD ModuleBase = Memory::GetModule(Module); //57D20000 + 0xA871D4
		//BYTE * ModuleBuff = (BYTE *)malloc(ModuleSize);
		DWORD BytesFound = 0;
		DWORD SizeOfMask = SizeOf;
		DWORD FinalAddre = 0;

		byte pattern[16] = { 0xE0, 0x19, 0xD1, 0x1D, 0xFD, 0x02, 0x00, 0x00, 0xC4, 0x71, 0x7A, 0x58, 0xD4, 0x75, 0x7A, 0x58 };
		//ReadProcessMemory(Memory::hProcess, LPCVOID(ModuleBase), &ModuleBuff, ModuleSize, NULL);

		for (x; x < ModuleSize; x++)
		{
			for (y; y < SizeOf; y++)
			{
				if (pattern[y] == '?')
					continue;

				if (pattern[y] != Memory::Read<byte>(ModuleBase + x + y))
					break;
				
				if (y == (SizeOf - 1) && pattern[y] == Memory::Read<byte>(ModuleBase + x + y))
				{
					FinalAddre = (x + y) - SizeOf;
					//free(&ModuleBuff);
					return FinalAddre;
				}
			}
		}
		//free(&ModuleBuff);
		return 0;
	}*/

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