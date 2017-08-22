#ifndef __CENGINE__
#define __CENGINE__

#ifdef _WIN32
#pragma once
#endif

#include "stdafx.h"

class CEngine
{
public:
	//uint32_t dwClientState;

	CEngine(void);
	static std::string GetMapDirectory(void);
	const char* GetMapName(void);
	static std::string GetGameDirectory(void);
};

extern CEngine* g_pEngine;

#endif