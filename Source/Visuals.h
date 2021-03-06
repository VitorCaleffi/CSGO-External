#ifndef __VISUALS__
#define __VISUALS__

#ifdef _WIN32
#pragma once
#endif

#include "stdafx.h"

class Visuals
{
private:
	DWORD	RadarStruct;
	float	Width, Height, pMatrix[ 16 ];
	Vector	Screen, Head, Foot, HeadScreen, FootScreen;
	int		c4Owner;
	char	CountRefresh = 200;

private:
	void	DrawPlayer( CBaseEntity* pEntity );

public:
	CBaseEntity* LocalPlayer;
	Visuals( void );
	void	Main( void );

};

extern Visuals* pVisuals;

#endif