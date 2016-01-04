#ifndef __STDAFX__
#define __STDAFX__

#ifdef _WIN32
#pragma once
#endif

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <d3d9.h>
#include <dwmapi.h>
#include <tchar.h>
#include <d3dx9.h>
#include <fstream>
#include <ctime>
#include <string>

#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )
#pragma comment( lib, "dwmapi.lib" )

#include "Helper.h"

#include "Vector.h"

#include "Math.h"

#include "Color.h"
#include "Direct3D9.h"

#include "Memory.h"

#include "SDK.h"

#endif