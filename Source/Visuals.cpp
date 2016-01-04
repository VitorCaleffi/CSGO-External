#include "Visuals.h"

Visuals* pVisuals = new Visuals();
//#define C4

void Visuals::DrawPlayer( CBaseEntity* pEntity )
{
	Color HealthColor,PlayerColor;

	PlayerColor = Color(255, 0, 0);

	if( !pEntity )
		return;

	Foot = pEntity->m_vecOrigin();
	Head = Foot + Vector( 0.f, 0.f, 72.f );

	if( pEntity->m_fFlags() & ( 1 << 1 ) )
		Head = Foot + Vector( 0.f, 0.f, 52.f );

	if( !WorldToScreen( Head, HeadScreen, pMatrix ) )
		return;

	if( !WorldToScreen( Foot, FootScreen, pMatrix ) )
		return;

	Height = FootScreen.y - HeadScreen.y;
	Width  = ( ( FootScreen.y - HeadScreen.y ) / 4.5f ) * 2.f;

	if (c4Owner == pEntity->iIndex && Help.MyTeam == 3)
	{
		PlayerColor = Color(255, 255, 140);
		Direct3D9::pRender->StringOutlined(10, 10, Color(255,255,255), Direct3D9::GetFont("Arial Bold"), "Bomber: %ls", pEntity->Name);
	}

	Direct3D9::pRender->BorderBox((int)(HeadScreen.x - Width / 2.f), (int)HeadScreen.y, (int)Width, (int)Height, 1, PlayerColor);
	//Direct3D9::pRender->StringOutlined((int)(HeadScreen.x - Width / 2.f), (int)(HeadScreen.y + Height + (Height / 100.f * 3)), 0xFFFFFFFF, Direct3D9::GetFont("Arial"), "%ls", pEntity->Name);
	//Direct3D9::pRender->StringOutlined((int)(HeadScreen.x + Width / 2.f + 3), (int)(HeadScreen.y), Color(255, 255, 0), Direct3D9::GetFont("Arial"), "%s", WeaponNames[pEntity->WeaponName()]);

	if (pEntity->m_iHealth() >= 60)
		HealthColor = D3DCOLOR_ARGB(255, 0, 255, 0, 0);
	else if (pEntity->m_iHealth() >= 30)
		HealthColor = D3DCOLOR_ARGB(255, 255, 255, 0, 0);
	else
		HealthColor = D3DCOLOR_ARGB(255, 255, 0, 0, 0);

	int Health = (Height) * ((float)pEntity->m_iHealth() / 100.f);

	Direct3D9::pRender->RectOutlined((int)(HeadScreen.x - Width), (int)(HeadScreen.y), 2, Health, HealthColor, 0xFF000000, 1);
}
void Visuals::DrawHitbox( CBaseEntity* pEntity, int iHitbox, Color color )
{
	Vector HitboxCorner[ ] =
	{ 
		Vector( Hitbox[ iHitbox ].vMin.x, Hitbox[ iHitbox ].vMin.y, Hitbox[ iHitbox ].vMax.z ),
		Vector( Hitbox[ iHitbox ].vMax.x, Hitbox[ iHitbox ].vMin.y, Hitbox[ iHitbox ].vMax.z ),
		Vector( Hitbox[ iHitbox ].vMin.x, Hitbox[ iHitbox ].vMin.y, Hitbox[ iHitbox ].vMin.z ),
		Vector( Hitbox[ iHitbox ].vMax.x, Hitbox[ iHitbox ].vMin.y, Hitbox[ iHitbox ].vMin.z ),
		Vector( Hitbox[ iHitbox ].vMin.x, Hitbox[ iHitbox ].vMax.y, Hitbox[ iHitbox ].vMax.z ),
		Vector( Hitbox[ iHitbox ].vMax.x, Hitbox[ iHitbox ].vMax.y, Hitbox[ iHitbox ].vMax.z ),
		Vector( Hitbox[ iHitbox ].vMin.x, Hitbox[ iHitbox ].vMax.y, Hitbox[ iHitbox ].vMin.z ),
		Vector( Hitbox[ iHitbox ].vMax.x, Hitbox[ iHitbox ].vMax.y, Hitbox[ iHitbox ].vMin.z )
	};

	Vector		TransformedVector[ 8 ]	= { Vector( 0.f, 0.f, 0.f ) };
	Vector		HitboxVector[ 8 ]		= { Vector( 0.f, 0.f, 0.f ) };
	
	matrix3x4_t TransMatrix;
	pEntity->getBoneData( Hitbox[ iHitbox ].iBone, TransMatrix );

	for( int i = 0; i < 8; i++ )
	{
		HitboxCorner[ i ] = HitboxCorner[ i ] * 0.85f;
		VectorTransform( HitboxCorner[ i ], TransMatrix, TransformedVector[ i ] );
		
		if( !WorldToScreen( TransformedVector[ i ], HitboxVector[ i ], pMatrix ) )
			return;
	}

	Direct3D9::pRender->Line( ( int )HitboxVector[ 0 ].x, ( int )HitboxVector[ 0 ].y, ( int )HitboxVector[ 1 ].x, ( int )HitboxVector[ 1 ].y, color, 0.5 );
	Direct3D9::pRender->Line( ( int )HitboxVector[ 2 ].x, ( int )HitboxVector[ 2 ].y, ( int )HitboxVector[ 3 ].x, ( int )HitboxVector[ 3 ].y, color, 0.5 ); 
	Direct3D9::pRender->Line( ( int )HitboxVector[ 4 ].x, ( int )HitboxVector[ 4 ].y, ( int )HitboxVector[ 5 ].x, ( int )HitboxVector[ 5 ].y, color, 0.5 );
	Direct3D9::pRender->Line( ( int )HitboxVector[ 6 ].x, ( int )HitboxVector[ 6 ].y, ( int )HitboxVector[ 7 ].x, ( int )HitboxVector[ 7 ].y, color, 0.5 );

	Direct3D9::pRender->Line( ( int )HitboxVector[ 0 ].x, ( int )HitboxVector[ 0 ].y, ( int )HitboxVector[ 2 ].x, ( int )HitboxVector[ 2 ].y, color, 0.5 );
	Direct3D9::pRender->Line( ( int )HitboxVector[ 1 ].x, ( int )HitboxVector[ 1 ].y, ( int )HitboxVector[ 3 ].x, ( int )HitboxVector[ 3 ].y, color, 0.5 );
	Direct3D9::pRender->Line( ( int )HitboxVector[ 4 ].x, ( int )HitboxVector[ 4 ].y, ( int )HitboxVector[ 6 ].x, ( int )HitboxVector[ 6 ].y, color, 0.5 );
	Direct3D9::pRender->Line( ( int )HitboxVector[ 5 ].x, ( int )HitboxVector[ 5 ].y, ( int )HitboxVector[ 7 ].x, ( int )HitboxVector[ 7 ].y, color, 0.5 );

	Direct3D9::pRender->Line( ( int )HitboxVector[ 0 ].x, ( int )HitboxVector[ 0 ].y, ( int )HitboxVector[ 4 ].x, ( int )HitboxVector[ 4 ].y, color, 0.5 );
	Direct3D9::pRender->Line( ( int )HitboxVector[ 1 ].x, ( int )HitboxVector[ 1 ].y, ( int )HitboxVector[ 5 ].x, ( int )HitboxVector[ 5 ].y, color, 0.5 );
	Direct3D9::pRender->Line( ( int )HitboxVector[ 2 ].x, ( int )HitboxVector[ 2 ].y, ( int )HitboxVector[ 6 ].x, ( int )HitboxVector[ 6 ].y, color, 0.5 );
	Direct3D9::pRender->Line( ( int )HitboxVector[ 3 ].x, ( int )HitboxVector[ 3 ].y, ( int )HitboxVector[ 7 ].x, ( int )HitboxVector[ 7 ].y, color, 0.5 );
}
Visuals::Visuals( void )
{
}
void Visuals::Main( void )
{

	Vector C4Vector;
	DWORD ActAddr,test = 0;
	char ActClassName[32];

	if (!Help.MyAddres)
		Help.MyAddres = Memory::Read<DWORD>(Memory::GetModule("client") + cLocalPlayer);

	//Help.MyTeam = Memory::Read<int>(Memory::Read<DWORD>(Memory::GetModule("client") + cLocalPlayer) + c_iTeamNum);

#ifdef C4
	if (!Help.ResourceAddr)
	{
		for (int i = 0; i < 1000; i++)
		{
			CBaseEntity* pEntity = new CBaseEntity(i);
			if (!strcmp(pEntity->ClassName, "CCSPlayerResource"))
			{
				Help.ResourceAddr = pEntity->dwEntity;
				break;
			}
		}
	}

	if (Help.ResourceAddr)
		c4Owner = Memory::Read<int>(Help.ResourceAddr + 0x1610); //m_iPlayerC4

	if (Help.MyTeam != 3 && Help.Defus)
	{
		CBaseEntity* pEntity = new CBaseEntity(Help.BombIndex);
		Direct3D9::pRender->StringOutlined(10, 10, 0xFF00FFFF, Direct3D9::GetFont("Arial"), "Defusando!!!");
	}
#endif
	//view
	ReadProcessMemory(Memory::hProcess, LPCVOID(Memory::GetModule("client") + cViewMatrix), &pMatrix, sizeof(pMatrix), NULL);
	//players

	for (int i = 0; i < 32; i++)
	{
		CBaseEntity* pEntity = new CBaseEntity(i);

		if (!pEntity->IsValid())
			continue;

		if (pEntity->m_iTeamNum() == Memory::Read<int>(Memory::Read<DWORD>(Memory::GetModule("client") + cLocalPlayer) + c_iTeamNum))
			continue;

		if (WorldToScreen(pEntity->m_vecOrigin(), Screen, pMatrix))
		{
			this->DrawPlayer(pEntity);
		}
	}
#ifdef C4
	if (c4Owner)
		Help.C4Planted = false;

	if (!c4Owner)
	{
		if (!Help.BombIndex)
		{
			for (int i = 0; i < 1000; i++)
			{
				CBaseEntity* pEntity = new CBaseEntity(i);

				if (!strcmp(pEntity->ClassName, (!Help.C4Planted) ? "CC4" : "CPlantedC4"))
				{
					Help.BombIndex = i;
					break;
				}
			}
			if (!Help.BombIndex)
				Help.C4Planted = !Help.C4Planted;
		}
		else
		{
			CBaseEntity* pEntity = new CBaseEntity(Help.BombIndex);

			if (strcmp(pEntity->ClassName, (!Help.C4Planted) ? "CC4" : "CPlantedC4"))
				Help.BombIndex = 0;

			if (pEntity->m_vecOrigin().x == 0 && pEntity->m_vecOrigin().y == 0)
				Help.BombIndex = 0;

			if (WorldToScreen(pEntity->m_vecOrigin(), C4Vector, pMatrix))
			{
				if (Help.C4Planted)
					Direct3D9::pRender->StringOutlined((int)C4Vector.x, (int)C4Vector.y, 0xFFFFFFFF, Direct3D9::GetFont("Arial"), "C4 Plantada");
				else
					Direct3D9::pRender->StringOutlined((int)C4Vector.x, (int)C4Vector.y, 0xFFFFFFFF, Direct3D9::GetFont("Arial"), "C4 Dropada");
			}
		}
	}

	if (!this->CountRefresh--)
	{
		Help.ResourceAddr = 0;
		Help.PlantedAddr = 0;
		this->CountRefresh = 200;
	}
#endif
}