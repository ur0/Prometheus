#pragma once
#include <Windows.h>

#include "MemoryManager.h"
#include "Offsets.h"
#include "Utils.h"

#include <ctime>
#include <thread>

DWORD WINAPI StartRadar(LPVOID);

class Radar
{
public:
	Radar(const int*);
	void Run();
	~Radar();

private:
	MemoryManager *MemMgr;
	DWORD ClientBaseAddr;
	const int *Control;
};

