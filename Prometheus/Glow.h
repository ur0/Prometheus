#pragma once

#include "MemoryManager.h"
#include "Offsets.h"
#include "Utils.h"

DWORD WINAPI GlowStart(LPVOID params);

class Glow
{
public:
	Glow(const int*);
	void Run();
	void Glow::SetGlow(int, float, float, float, float);
	~Glow();

private:
	MemoryManager *MemMgr;
	DWORD ClientBaseAddr;
	const int *Control;
};

