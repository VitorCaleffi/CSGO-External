#include "stdafx.h"
Offsets_t Offsets = {0, 0, 0, 0, 0, 0};

std::unique_ptr< Valve::BSPParser > g_pBSPParser = std::make_unique< Valve::BSPParser >();

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	//ErasePEHeaderFromMemory(); //fecha o cabeçalho
	//CanOpenCsrss(); //fecha conexão com csrss

	if( Memory::Initialize( "Counter-Strike: Global Offensive" ) )
	{
		Memory::AddModule( "client", "client.dll" );
		Memory::AddModule( "engine", "engine.dll" );

		Offsets.DwViewMatrix = Memory::FindPattern("client", "81 C6 ? ? ? ? 88 45 9A 0F B6 C0", 0x352, 0xB0);
		Offsets.cLocalPlayer = Memory::FindPattern("client", "A3 ? ? ? ? C7 05 ? ? ? ? ? ? ? ? E8 ? ? ? ? 59 C3 6A ?", 0x1, 0x2C);
		Offsets.cRadarStruct = Memory::FindPattern("client", "A1 ? ? ? ? 8B 0C B0 8B 01 FF 50 ? 46 3B 35 ? ? ? ? 7C EA 8B 0D", 0x1, 0x0);
		Offsets.cEntityList = Memory::FindPattern("client", "BB ? ? ? ? 83 FF 01 0F 8C ? ? ? ? 3B F8", 0x1, 0x0);
		Offsets.DwClientState = Memory::FindPattern("engine", "A1 ? ? ? ? F3 0F 11 80 ? ? ? ? D9 46 04 D9 05", 0x1, 0x0);
		Offsets.DwGameDir = Memory::FindPattern("engine", "68 ? ? ? ? 8D 85 ? ? ? ? 50 68 ? ? ? ? 68", 0x1, 0x0);
		Offsets.DwMapName = Memory::FindPattern("engine", "05 ? ? ? ? C3 CC CC CC CC CC CC CC A1", 0x1, 0x0);
		Offsets.DwMapDir = Memory::FindPattern("engine", "05 ? ? ? ? C3 CC CC CC CC CC CC CC 80 3D", 0x1, 0x0);
		

		if (g_pBSPParser->parse_map(CEngine::GetGameDirectory(), CEngine::GetMapDirectory())) {
			static std::string last_map;
			auto bsp_file = g_pBSPParser->get_bsp();
			if (last_map != bsp_file.m_FileName) {
				last_map = bsp_file.m_FileName;
			}
		}
	}
	else
	{
		MessageBoxA( NULL, "Erro ao inciiasdasd fodase", "Ef", MB_ICONERROR );
		return NULL;
	}

	if( SUCCEEDED( Direct3D9::CreateOverlay( "Counter-Strike: Global Offensive", "jasmaissabera" ) ) )
	{
		while( Direct3D9::Render() )
		{
			Sleep( 5 );
		}
	}
	return NULL;
}