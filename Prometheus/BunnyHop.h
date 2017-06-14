#pragma once
#include <Windows.h>

#include "MemoryManager.h"
#include "Offsets.h"
#include "Utils.h"

DWORD WINAPI BHopStart(LPVOID params);

class BunnyHop
{
public:
	BunnyHop(const int*);
	void Run();
	~BunnyHop();

private:
	MemoryManager *MemMgr;
	DWORD ClientBaseAddr;
	const int *Control;
};

