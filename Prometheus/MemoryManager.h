#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include "Utils.h"
#include <vector>

class MemoryManager
{
public:
	MemoryManager();
	~MemoryManager();
	MemoryManager(const std::wstring&);
	bool Attach(const std::wstring&);
	bool GrabModule(const std::wstring&);
	HANDLE MemoryManager::GetHandle();
	DWORD MemoryManager::GetProcId();
	std::vector<MODULEENTRY32> MemoryManager::GetModules();

	template <class T> inline void Read(DWORD dwAddr, T& Value) {
		if (!ReadProcessMemory(m_hProcess, (LPVOID)dwAddr, reinterpret_cast<LPVOID>(&Value), sizeof(T), NULL)) {
			printf("Address: 0x%x failed reading!\n", dwAddr);
		}
	}
	template <class T> inline void Write(DWORD dwAddr, T& Value) {
		if (!WriteProcessMemory(m_hProcess, (LPVOID)dwAddr, reinterpret_cast<LPVOID>(&Value), sizeof(T), NULL)) {
			printf("Address: 0x%x failed writing!\n", dwAddr);
		}
	}


private:
	HANDLE m_hProcess;
	DWORD m_dwProcessId;
	std::vector<MODULEENTRY32> m_Modules;
};

size_t GetClientBaseAddr(MemoryManager *);
size_t GetEngineBaseAddr(MemoryManager *);