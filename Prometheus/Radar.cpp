#include "stdafx.h"
#include "Radar.h"

DWORD WINAPI StartRadar(LPVOID param) {
	Radar r = Radar((const int*)param);
	r.Run();
	return 0;
}


Radar::Radar(const int* control)
{
	// Initialize the Memory Manager
	MemMgr = new MemoryManager(L"csgo.exe");
	ClientBaseAddr = GetClientBaseAddr(MemMgr);
	Control = control;
}

void Radar::Run() {
	DWORD entity;
	bool t = true, status = false;
	while (true) {
		if (*Control) {
			for (int i = 0; i < 65; i++) {
				MemMgr->Read(ClientBaseAddr + O_ENTITY_LIST + (i - 1) * O_ENT_LOOP_DIST, entity);
				if (entity) {
					MemMgr->Read(entity + O_M_BVISIBLE, status);
					if (!status) {
						MemMgr->Write(entity + O_M_BVISIBLE, t);
					}
				}
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}


Radar::~Radar()
{
}
