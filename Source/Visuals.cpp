#include "Visuals.h"

Visuals* pVisuals = new Visuals();

void Visuals::DrawPlayer( CBaseEntity* pEntity )
{
	Color HealthColor,PlayerColor;

	if( !pEntity )
		return;
	
	PlayerColor = g_pBSPParser->is_visible(this->LocalPlayer->ValveEyePosition(), pEntity->ValveEyePosition()) ? Color(255, 255, 0) : Color(255, 0, 0);

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

	Direct3D9::pRender->BorderBox((int)(HeadScreen.x - Width / 2.f), (int)HeadScreen.y, (int)Width, (int)Height, 1, PlayerColor);
	Direct3D9::pRender->StringOutlined((int)(HeadScreen.x - Width / 2.f), (int)(HeadScreen.y + Height + (Height / 100.f * 3)), 0xFFFFFFFF, Direct3D9::GetFont("Arial"), "%ls", pEntity->Name);

	if (pEntity->m_iHealth() >= 60)
		HealthColor = D3DCOLOR_ARGB(255, 0, 255, 0, 0);
	else if (pEntity->m_iHealth() >= 30)
		HealthColor = D3DCOLOR_ARGB(255, 255, 255, 0, 0);
	else
		HealthColor = D3DCOLOR_ARGB(255, 255, 0, 0, 0);

	int Health = (Height) * ((float)pEntity->m_iHealth() / 100.f);

	Direct3D9::pRender->RectOutlined((int)(HeadScreen.x - Width), (int)(HeadScreen.y), 2, Health, HealthColor, 0xFF000000, 1);
}

Visuals::Visuals( void )
{
}

void Visuals::Main( void )
{
	this->LocalPlayer = new CBaseEntity(0);
	//view
	ReadProcessMemory(Memory::hProcess, LPCVOID(Memory::GetModule("client") + Offsets.DwViewMatrix), &pMatrix, sizeof(pMatrix), NULL);

	//players
	for (int i = 1; i < 32; i++)
	{
		CBaseEntity* pEntity = new CBaseEntity(i);

		if (!pEntity->IsValid())
			continue;
		 
		if (pEntity->m_iTeamNum() == this->LocalPlayer->m_iTeamNum())
			continue;

		if (WorldToScreen(pEntity->m_vecOrigin(), Screen, pMatrix))
			this->DrawPlayer(pEntity);
	}
}