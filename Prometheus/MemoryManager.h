#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <vector>

class MemoryManager
{
public:
	MemoryManager();
	~MemoryManager();
	MemoryManager(const std::wstring&);
	bool Attach(const std::wstring&);
	bool GrabModule(const std::wstring&);
	template <class T> bool Read(DWORD, T&);
	template <class T> bool Write(DWORD, T&);
	HANDLE MemoryManager::GetHandle();
	DWORD MemoryManager::GetProcId();
	std::vector<MODULEENTRY32> MemoryManager::GetModules();


private:
	HANDLE m_hProcess;
	DWORD m_dwProcessId;
	std::vector<MODULEENTRY32> m_Modules;
};
