#ifndef __MEMORY__
#define __MEMORY__

#ifdef _WIN32
#pragma once
#endif

#include "stdafx.h"

#define MAX_MEMORY_MODULE	3

namespace Memory
{
	extern HANDLE	hProcess;
	extern HWND		hWindow;

	extern bool		Initialize(std::string szGameWindowTitle);
	extern DWORD	GetModule(std::string szName);
	extern void		AddModule(std::string szName, std::string szModuleName);
	extern bool		HideThread(HANDLE hThread);
	extern DWORD GetModuleSize(std::string szModuleName);
	extern DWORD FindPattern(BYTE * ArrayOfBytes, std::string Module, int SizeOf);
	extern DWORD FindPattern(HANDLE process, DWORD start, DWORD size, const char* sig, const char* mask, DWORD pattern_offset, DWORD address_offset);


	template<class T> T Read(DWORD dwBaseAddress)
	{
		T TBuffer;
		ReadProcessMemory(hProcess, LPCVOID(dwBaseAddress), &TBuffer, sizeof(T), NULL);
		return TBuffer;
	}
	template<class T> BOOL Write(DWORD dwBaseAddress, T ValueToWrite)
	{
		return WriteProcessMemory(hProcess, LPVOID(dwBaseAddress), &ValueToWrite, sizeof(T), NULL);
	}
};

#endif