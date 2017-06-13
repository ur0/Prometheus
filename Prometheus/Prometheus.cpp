// Prometheus.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MemoryManager.h"
#include "Utils.h"
#include <ctime>
#include <thread>

int main()
{
	// Initialize the Memory Manager
	MemoryManager *MemMgr = new MemoryManager(L"csgo.exe");

	if (!MemMgr->GrabModule(L"client.dll")) {
		Utils::ErrorAndExit("Couldn't find client.dll!");
	}

	// Find client.dll's base address
	DWORD dwClientBase = 0;
	for (auto m : MemMgr->GetModules())
	{
		if (!wcscmp(m.szModule, L"client.dll"))
		{
			dwClientBase = reinterpret_cast<DWORD>(m.modBaseAddr);
			break;
		}
	}
	if (!dwClientBase)
		Utils::ErrorAndExit("Couldn't find client.dll's base address");

	return 0;
}
