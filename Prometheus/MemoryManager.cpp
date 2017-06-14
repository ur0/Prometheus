#include "stdafx.h"
#include "MemoryManager.h"
#include "Utils.h"

MemoryManager::MemoryManager()
{
	m_hProcess = INVALID_HANDLE_VALUE;
	m_dwProcessId = 0;
	m_Modules.clear();
}

MemoryManager::MemoryManager(const std::wstring& strProcessName) {
	m_hProcess = INVALID_HANDLE_VALUE;
	m_dwProcessId = 0;
	m_Modules.clear();

	if (!Attach(strProcessName))
		Utils::ErrorAndExit("You need to start the game, duh!");
}

bool MemoryManager::Attach(const std::wstring& strProcessName) {
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (hSnapshot == INVALID_HANDLE_VALUE) return false;

	PROCESSENTRY32 ProcEntry;
	ProcEntry.dwSize = sizeof(PROCESSENTRY32);

	if (Process32First(hSnapshot, &ProcEntry)) {

		if (!wcscmp(ProcEntry.szExeFile, strProcessName.c_str()))
		{
			CloseHandle(hSnapshot);
			m_hProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE, ProcEntry.th32ProcessID);
			m_dwProcessId = ProcEntry.th32ProcessID;
			return true;
		}
	}
	else {
		CloseHandle(hSnapshot);
		return false;
	}

	while (Process32Next(hSnapshot, &ProcEntry)) {
		if (!wcscmp(ProcEntry.szExeFile, (wchar_t *)strProcessName.c_str()))
		{
			m_hProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE, ProcEntry.th32ProcessID);
			m_dwProcessId = ProcEntry.th32ProcessID;
			return true;
		}
	}
	CloseHandle(hSnapshot);
	return false;
}

bool MemoryManager::GrabModule(const std::wstring& strModuleName)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, m_dwProcessId);
	if (hSnapshot == INVALID_HANDLE_VALUE) return false;

	MODULEENTRY32 ModEntry;
	ModEntry.dwSize = sizeof(MODULEENTRY32);

	if (Module32First(hSnapshot, &ModEntry))
	{
		if (!wcscmp(ModEntry.szModule, strModuleName.c_str()))
		{
			CloseHandle(hSnapshot);
			m_Modules.push_back(ModEntry);
			return true;
		}
	}
	else
	{
		CloseHandle(hSnapshot);
		return false;
	}

	while (Module32Next(hSnapshot, &ModEntry))
	{
		if (!wcscmp(ModEntry.szModule, strModuleName.c_str()))
		{
			CloseHandle(hSnapshot);
			m_Modules.push_back(ModEntry);
			return true;
		}
	}

	CloseHandle(hSnapshot);
	return false;
}

HANDLE MemoryManager::GetHandle() { return m_hProcess; }
DWORD MemoryManager::GetProcId() { return m_dwProcessId; }
std::vector<MODULEENTRY32> MemoryManager::GetModules() { return m_Modules; }

size_t GetClientBaseAddr(MemoryManager *MemMgr) {
	// Load client.dll
	if (!MemMgr->GrabModule(L"client.dll")) {
		Utils::ErrorAndExit("Couldn't find client.dll!");
	}

	// Find client.dll's base address
	size_t pClientBase = 0;
	for (auto m : MemMgr->GetModules())
	{
		if (!wcscmp(m.szModule, L"client.dll"))
		{
			pClientBase = reinterpret_cast<size_t>(m.modBaseAddr);
			break;
		}
	}
	if (!pClientBase)
		Utils::ErrorAndExit("Couldn't find client.dll's base address");

	return pClientBase;
}

MemoryManager::~MemoryManager()
{
}
