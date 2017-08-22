#ifndef __MEMORY__
#define __MEMORY__

#ifdef _WIN32
#pragma once
#endif

#include "stdafx.h"

#define MAX_MEMORY_MODULE	3

#define INRANGE(x,a,b)		(x >= a && x <= b) 
#define getBits( x )		(INRANGE(x,'0','9') ? (x - '0') : ((x&(~0x20)) - 'A' + 0xa))
#define getByte( x ) (getBits(x[0]) << 4 | getBits(x[1]))

namespace Memory
{
	extern HANDLE	hProcess;
	extern HWND		hWindow;

	extern bool		Initialize(std::string szGameWindowTitle);
	extern DWORD	GetModule(std::string szName);
	extern DWORD GetModuleSize(std::string szModuleName);
	extern void		AddModule(std::string szName, std::string szModuleName);
	//extern bool		HideThread(HANDLE hThread);
	extern DWORD GetModuleSize(std::string szModuleName);
	extern uintptr_t FindPattern(const std::string& module, const char* pattern, uintptr_t patternOffset, uintptr_t addressOffset);


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