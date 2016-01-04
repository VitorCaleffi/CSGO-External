#ifndef __HITBOX__
#define __HITBOX__

#ifdef _WIN32
#pragma once
#endif

#include "stdafx.h"

#ifndef HITBOX_MAX
#define HITBOX_MAX	18
#endif

enum Hibtox_e
{
	PELVIS = 0,	
	L_THIGH,
	L_CALF,
	L_FOOT,
	R_THIGH,
	R_CALF,
	R_FOOT,
	SPINE2,
	SPINE3,
	SPINE4,
	NECK,
	HEAD,
	L_UPPERARM,
	L_FOREARM,
	L_HAND,
	R_UPPERARM,
	R_FOREARM,
	R_HAND,
};

struct Hitbox_t
{
	int		iBone;
	Vector	vMin;
	Vector	vMax;

};
extern Hitbox_t	Hitbox[ HITBOX_MAX ];
extern void		SetupHitbox( void );

#endif