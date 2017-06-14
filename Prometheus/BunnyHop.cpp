#include "stdafx.h"

#include <ctime>
#include <thread>

#include "BunnyHop.h"

// Function for CreateThread
DWORD WINAPI BHopStart(LPVOID control) {
	BunnyHop bhop = BunnyHop((int *)control);
	bhop.Run();
	return 0;
}

BunnyHop::BunnyHop(const int *control)
{
	// Initialize the Memory Manager
	MemMgr = new MemoryManager(L"csgo.exe");
	ClientBaseAddr = GetClientBaseAddr(MemMgr);
	Control = control;
}

void BunnyHop::Run() {
	// For storing flags
	uint8_t m_fFlags;
	// Acquire a pointer to the local player
	size_t pLocalPlayer;
	MemMgr->Read(ClientBaseAddr + O_LOCAL_PLAYER, pLocalPlayer);

	while (true) {
		if (!*Control) {
			// We're still sleeping because we need to wait before polling again
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			continue;
		}

		// Get m_fFlags for the player
		MemMgr->Read(pLocalPlayer + O_M_FFLAGS, m_fFlags);

		if (m_fFlags && (1 << 0)) {
			// We're on the ground
			INPUT Input;
			ZeroMemory(&Input, sizeof(Input));
			Input.type = INPUT_KEYBOARD;
			Input.ki.dwFlags = KEYEVENTF_SCANCODE;
			Input.ki.wScan = 0x39;
			SendInput(1, &Input, sizeof(INPUT));

			ZeroMemory(&Input, sizeof(Input));
			Input.type = INPUT_KEYBOARD;
			Input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
			Input.ki.wScan = 0x39;
			SendInput(1, &Input, sizeof(INPUT));
		}

		// Don't put the CPU on fire
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

BunnyHop::~BunnyHop()
{
}
