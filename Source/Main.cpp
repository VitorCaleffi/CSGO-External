#include "stdafx.h"

Helper_t Help = { 0,0,0, 0, 0, 0 , false, 0};

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	//ErasePEHeaderFromMemory(); //fecha o cabeçalho
	//CanOpenCsrss(); //fecha conexão com csrss

	if( Memory::Initialize( "Counter-Strike: Global Offensive" ) )
	{
		Memory::AddModule( "client", "client.dll" );
		Memory::AddModule( "engine", "engine.dll" );
	}
	else
	{
		MessageBoxA( NULL, "Erro ao inciiasdasd fodase", "Ef", MB_ICONERROR );
		return NULL;
	}

	SetupHitbox();

	if( SUCCEEDED( Direct3D9::CreateOverlay( "Counter-Strike: Global Offensive", "jasmaissabera" ) ) )
	{
		while( Direct3D9::Render() )
		{
			Sleep( 5 );
		}
	}
	return NULL;
}