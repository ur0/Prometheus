#include "stdafx.h"
#include "Glow.h"

#include <ctime>
#include <thread>

DWORD WINAPI GlowStart(LPVOID params) {
	Glow glow = Glow((const int*)params);
	glow.Run();
	return 0;
}

Glow::Glow(const int* control)
{
	// Initialize the Memory Manager
	MemMgr = new MemoryManager(L"csgo.exe");
	ClientBaseAddr = GetClientBaseAddr(MemMgr);
	Control = control;
}

void Glow::Run() {
	size_t pLocalPlayer;
	MemMgr->Read(ClientBaseAddr + O_LOCAL_PLAYER, pLocalPlayer);
	while (true)
	{
		if (*Control) {
			for (int i = 1; i <= 64; i++)
			{
				DWORD dwEntity;
				MemMgr->Read(ClientBaseAddr + O_ENTITY_LIST + (i - 1) * O_ENT_LOOP_DIST, dwEntity);
				if (!dwEntity)
					continue;
				int iEntityTeam;
				MemMgr->Read(dwEntity + O_M_I_TEAM_NUM, iEntityTeam);
				int iTargetHealth;
				MemMgr->Read(dwEntity + O_M_I_HEALTH, iTargetHealth);
				int iLocalPlayerTeam;
				MemMgr->Read(pLocalPlayer + O_M_I_TEAM_NUM, iLocalPlayerTeam);

				if (iTargetHealth > 0)
				{
					int iGlowIndex;
					MemMgr->Read<int>(dwEntity + O_M_I_GLOW_IDX, iGlowIndex);
					if (iEntityTeam == iLocalPlayerTeam)
						SetGlow(iGlowIndex, 0, 0, 1, 0.8);
					else 
						SetGlow(iGlowIndex, 1, 0, 0, 0.8);
				}
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(25));
	}
}

void inline Glow::SetGlow(int iEntityGlowIndex, float r, float g, float b, float a)
{
	size_t dwGlowObjectManager;
	MemMgr->Read(ClientBaseAddr + O_GLOW_OBJ_MJR, dwGlowObjectManager);
	MemMgr->Write(dwGlowObjectManager + ((iEntityGlowIndex * 0x38) + 0x4), r);
	MemMgr->Write(dwGlowObjectManager + ((iEntityGlowIndex * 0x38) + 0x8), g);
	MemMgr->Write(dwGlowObjectManager + ((iEntityGlowIndex * 0x38) + 0xC), b);
	MemMgr->Write(dwGlowObjectManager + ((iEntityGlowIndex * 0x38) + 0x10), a);

	bool t = true, f = false;
	MemMgr->Write(dwGlowObjectManager + ((iEntityGlowIndex * 0x38) + 0x24), t);
	MemMgr->Write(dwGlowObjectManager + ((iEntityGlowIndex * 0x38) + 0x25), f);
}

Glow::~Glow()
{
}
