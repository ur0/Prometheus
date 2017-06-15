#include "stdafx.h"
#include "AimBot.h"


DWORD WINAPI StartAimBot(LPVOID param) {
	AimBot ab = AimBot((const int*)param);
	ab.Run();
	return 0;
}

AimBot::AimBot(const int* control) {
	// Initialize the Memory Manager
	MemMgr = new MemoryManager(L"csgo.exe");
	ClientBaseAddr = GetClientBaseAddr(MemMgr);
	Control = control;
}

void AimBot::Run() {
	DWORD dwTarget = 0x6969, dwLocalPlayer, dwEngineState;
	Vec3 playerPos, delta;
	int iLocalPlayerTeam, iTargetTeam, iTargetHealth, iCrosshair;
	bool breakout = false;

	MemMgr->Read(ClientBaseAddr + O_LOCAL_PLAYER, dwLocalPlayer);
	MemMgr->Read(dwLocalPlayer + O_M_I_TEAM_NUM, iLocalPlayerTeam);
	MemMgr->Read(GetEngineBaseAddr(MemMgr) + O_M_DW_CLIENT_STATE, dwEngineState);

	while (true) {
		MemMgr->Read(ClientBaseAddr + O_M_VEC_ORIGIN, playerPos);
		MemMgr->Read(dwLocalPlayer + O_M_ICROSSHAIR, iCrosshair);
		MemMgr->Read(ClientBaseAddr + O_ENTITY_LIST + (iCrosshair - 1) * O_ENT_LOOP_DIST, dwTarget);
		MemMgr->Read(dwTarget + O_M_I_TEAM_NUM, iTargetTeam);
		MemMgr->Read(dwTarget + O_M_I_HEALTH, iTargetHealth);


		if (dwTarget != 0x6969 && iTargetHealth > 0 && (iLocalPlayerTeam != iTargetTeam)) {
			// Aim at this target
			Vec3 punch, eyePos, oldAngle;
			MemMgr->Read(dwTarget + O_M_I_HEALTH, iTargetHealth);
			while (iTargetHealth > 0) {
				if (breakout) {
					breakout = false;
					break;
				}

				MemMgr->Read(dwLocalPlayer + O_M_PUNCH, punch);
				MemMgr->Read(dwLocalPlayer + O_M_VEC_ORIGIN, eyePos);
				MemMgr->Read(dwLocalPlayer + O_M_VEC_VIEW_OFFSET, delta);
				MemMgr->Read(dwEngineState + O_M_VIEW_ANGLES, oldAngle);

				punch.x *= 2;
				punch.y *= 2;
				punch.z *= 2;
				eyePos.z += delta.z;

				Vec3 aimAngle = GetNewAngle(eyePos, GetBonePos(dwTarget, BONE_ID_TO_AIM));
				// Adjust recoil
				aimAngle.x -= punch.x;
				aimAngle.y -= punch.y;
				aimAngle.z -= punch.z;
				aimAngle = Normalize(aimAngle);
				
				Vec3 smoothedAngle = VecSmooth(oldAngle, aimAngle, SMOOTH_AMOUNT);
				smoothedAngle = Normalize(smoothedAngle);
				MemMgr->Write(dwEngineState + O_M_VIEW_ANGLES, smoothedAngle);

				// Update the health so that this stays sane
				MemMgr->Read(dwTarget + O_M_I_HEALTH, iTargetHealth);

				// Manual breakout if shit hits the fan
				if (GetAsyncKeyState(VK_F1)) {
					std::this_thread::sleep_for(std::chrono::milliseconds(3000));
					breakout = true;
				}

				// This will make sure that we're aimed in 0.5s
				std::this_thread::sleep_for(std::chrono::milliseconds(20));
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}

// I have no idea what this does
Vec3 AimBot::GetNewAngle(Vec3 v1, Vec3 v2) {
	Vec3 res, v2d, fin;

	res.x = v2.x - v1.x;
	res.y = v2.y - v1.y;
	res.z = v2.z - v1.z;
	v2d = res;
	v2d.z = 0;

	float v2dm = sqrtf((v2d.x * v2d.x) + (v2d.y * v2d.y));

	fin.x = (float)(atan2(-res.z, v2dm) * 180 / M_PI);
	fin.y = (float)(atan2(res.y, res.x) * 180 / M_PI);
	fin.z = 0;

	return fin;
}

// Get the distance to target
float AimBot::GetAimDist(Vec3 eyePos, Vec3 playerHeadPos, Vec3 entityHeadPos) {
	Vec3 newAngles = GetNewAngle(playerHeadPos, entityHeadPos);

	float yaw = ((float)sin(fabsf(eyePos.y - newAngles.y) * M_PI / 180)) * VectorDistance(playerHeadPos, entityHeadPos);
	float pitch = ((float)sin(fabsf(eyePos.x - newAngles.x) * M_PI / 180)) * VectorDistance(playerHeadPos, entityHeadPos);

	return sqrt(yaw*yaw + pitch*pitch);
}

float AimBot::VectorDistance(Vec3 vector1, Vec3 vector2) {
	return sqrt(pow((vector1.x - vector2.x), 2) + pow((vector1.y - vector2.y), 2) + pow((vector1.z - vector2.z), 2));
}

Vec3 AimBot::GetBonePos(DWORD dwEntity, int iTargetBone) {
	Vec3 bonePos;
	DWORD boneMtx;

	MemMgr->Read(dwEntity + O_M_DW_BONE_MATRIX, boneMtx);
	MemMgr->Read(boneMtx + (0x30 * iTargetBone) + 0x0c, bonePos.x);
	MemMgr->Read(boneMtx + (0x30 * iTargetBone) + 0x1c, bonePos.y);
	MemMgr->Read(boneMtx + (0x30 * iTargetBone) + 0x2c, bonePos.z);

	return bonePos;
}

Vec3 AimBot::VecSmooth(Vec3 viewAngles, Vec3 newAngles, float smoothAmount) {
	Vec3 diff1, fin;

	diff1.x = (newAngles.x - viewAngles.x) / smoothAmount;
	diff1.y = (newAngles.y - viewAngles.y) / smoothAmount;
	diff1.z = (newAngles.z - viewAngles.z) / smoothAmount;
	
	fin.x = diff1.x + viewAngles.x;
	fin.y = diff1.y + viewAngles.y;
	fin.z = diff1.z + viewAngles.z;

	return fin;
}

Vec3 AimBot::Normalize(Vec3 angles) {
	if (angles.x > 89.0f && angles.x <= 180.0f)
		angles.x = 89.0f;

	while (angles.x > 180.f)
		angles.x -= 360.f;

	while (angles.x < -89.0f)
		angles.x = -89.0f;

	while (angles.y > 180.f)
		angles.y -= 360.f;

	while (angles.y < -180.f)
		angles.y += 360.f;

	angles.z = 0;
	return angles;
}

AimBot::~AimBot()
{
}
