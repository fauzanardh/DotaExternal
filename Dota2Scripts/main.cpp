#include <iostream>
#include <cstdio>
#include <vector>
#include <Windows.h>
#include "Memory.h"


struct Modules
{
	uintptr_t clientModule;
	uintptr_t serverModule;
} modules;

int main()
{	
	DWORD processId = GetProcId(L"dota2.exe");

	HANDLE hProcess = 0;
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, processId);

	modules.clientModule = GetModuleBaseAddress(processId, L"client.dll");
	modules.serverModule = GetModuleBaseAddress(processId, L"server.dll");

	std::cout << "Found clientModule => 0x" << std::hex << modules.clientModule << std::endl;
	std::cout << "Found serverModule => 0x" << std::hex << modules.serverModule << std::endl;

	uintptr_t heroPointer = modules.serverModule + 0x024F5EE8;

	std::cout << "Found hero pointer => 0x" << std::hex << heroPointer << std::endl;

	std::vector<unsigned int> currentManaOffsets = {0x0, 0x1C0, 0x18, 0x658};
	uintptr_t currentManaAddress = FindDMAAddy(hProcess, heroPointer, currentManaOffsets);

	float currentMana = 0;

	while (true) {
		ReadProcessMemory(hProcess, (BYTE*)currentManaAddress, &currentMana, sizeof(currentMana), nullptr);
		printf("Current mana of the hero: %.1f\r", currentMana);
		Sleep(100);
	}

	return 0;
}