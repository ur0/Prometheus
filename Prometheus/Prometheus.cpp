// Prometheus.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MemoryManager.h"
#include "Utils.h"

int main()
{
	// Initialize the Memory Manager
	MemoryManager *MemMgr = new MemoryManager(L"csgo.exe");

	if (!MemMgr->GrabModule(L"client.dll")) {
		Utils::ErrorAndExit("Couldn't find client.dll!");
	}

    return 0;
}
