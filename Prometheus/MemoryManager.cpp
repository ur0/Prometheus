#include "stdafx.h"
#include "MemoryManager.h"


MemoryManager::MemoryManager()
{
	m_hProcess = INVALID_HANDLE_VALUE;
	m_dwProcessId = 0;
	m_Modules.clear();
}

MemoryManager::MemoryManager(const std::wstring& strProcessName = L"csgo.exe") {
	m_hProcess = INVALID_HANDLE_VALUE;
	m_dwProcessId = 0;
	m_Modules.clear();

	if (!Attach(strProcessName))
		throw;
}

bool MemoryManager::Attach(const std::wstring& strProcessName) {
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (hSnapshot == INVALID_HANDLE_VALUE) return false;

	PROCESSENTRY32 ProcEntry;
	ProcEntry.dwSize = sizeof(PROCESSENTRY32);

	if (Process32First(hSnapshot, &ProcEntry)) {

		if (!wcscmp(ProcEntry.szExeFile, (wchar_t *)strProcessName.c_str()))
		{
			CloseHandle(hSnapshot);
			m_hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcEntry.th32ProcessID);
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
			m_dwProcessId = ProcEntry.th32ProcessID;
			return true;
		}
	}
	CloseHandle(hSnapshot);
	return false;
}

template <class T> bool MemoryManager::Read(DWORD dwAddr, T& Value) {
	return ReadProcessMemory(m_hProcess, reinterpret_cast<LPVOID>(dwAddress), Value, sizeof(T), NULL) == TRUE;
}

template <class T> bool MemoryManager::Write(DWORD dwAddr, T& Value) {
	return WriteProcessMemory(m_hProcess, reinterpret_cast<LPVOID>(dwAddress), Value, sizeof(T), NULL) == TRUE;
}

HANDLE MemoryManager::GetHandle() { return m_hProcess; }
DWORD MemoryManager::GetProcId() { return m_dwProcessId; }
std::vector<MODULEENTRY32> MemoryManager::GetModules() { return m_Modules; }

MemoryManager::~MemoryManager()
{
}
