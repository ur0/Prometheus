#pragma once

#include <Windows.h>
#include <math.h>

#include <ctime>
#include <thread>

#include "MemoryManager.h"
#include "Offsets.h"

struct Vec3 {
	float x;
	float y;
	float z;
};

DWORD WINAPI StartAimBot(LPVOID params);

class AimBot
{
public:
	AimBot(const int *control);
	void Run();
	~AimBot();

private:
	MemoryManager *MemMgr;
	const int *Control;
	DWORD ClientBaseAddr;
	DWORD EngineBaseAddr;

	Vec3 GetNewAngle(Vec3, Vec3);
	Vec3 GetBonePos(DWORD, int);
	Vec3 VecSmooth(Vec3, Vec3, float);
	Vec3 Normalize(Vec3);
};

