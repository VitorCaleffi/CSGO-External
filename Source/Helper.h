#ifndef HELPER_H
#define HELPER_H

#include "stdafx.h"

struct Offsets_t
{
	uintptr_t		DwViewMatrix;
	uintptr_t		cLocalPlayer;
	uintptr_t		cRadarStruct;
	uintptr_t		cEntityList;
	uintptr_t		DwClientState;
	uintptr_t		DwGameDir;
	uintptr_t		DwMapName;
	uintptr_t		DwMapDir;
};

extern Offsets_t Offsets;

#endif