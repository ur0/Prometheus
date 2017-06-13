#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <vector>

// I don't know why but VS doesn't like me
HANDLE WINAPI CreateToolhelp32Snapshot(DWORD, DWORD);
BOOL WINAPI Process32FirstW(HANDLE, LPPROCESSENTRY32);
BOOL WINAPI Process32NextW(HANDLE, LPPROCESSENTRY32);

class MemoryManager
{
public:
	MemoryManager();
	~MemoryManager();
	MemoryManager(const std::wstring&);
	bool Attach(const std::wstring&);
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
