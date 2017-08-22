#include "CEngine.h"

CEngine* g_pEngine = new CEngine();

CEngine::CEngine()
{
	//this->dwClientState = Memory::GetModule("engine") + Offsets.DwClientState;
}

std::string CEngine::GetMapDirectory(void)
{
	static std::array< char, 0x120  > map_directory;

	map_directory = Memory::Read<std::array< char, 0x120 > >(Memory::Read<uint32_t>(Memory::GetModule("engine") + Offsets.DwClientState) + 0x180);

	return std::string(map_directory.data());
}

const char* CEngine::GetMapName(void)
{
	static std::array< char, 0x80 > map_name;
	map_name = Memory::Read<std::array< char, 0x80 > >(Memory::Read<uint32_t>(Memory::GetModule("engine") + Offsets.DwClientState) + 0x284);
	
	return map_name.data();
}

std::string CEngine::GetGameDirectory(void)
{
	static std::array< char, 0x120 > game_directory;
	ReadProcessMemory(Memory::hProcess, LPCVOID(Memory::GetModule("engine") + Offsets.DwGameDir), game_directory.data(), game_directory.size(), NULL);

	return std::string(game_directory.data()).append("\\");
}