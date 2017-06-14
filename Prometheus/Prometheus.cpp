// Prometheus.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MemoryManager.h"
#include "Utils.h"

#include <ctime>
#include <iostream>
#include <thread>

#include "BunnyHop.h"

size_t GetClientBaseAddr(MemoryManager *);

void ShowUsage() {
	std::cout << "Prometheus v<1.00> by ur0" << std::endl;
	std::cout << "Usage:" << std::endl;
	std::cout << "Bunny Hop - <F6> to enable, <F7> to disable" << std::endl;
	std::cout << "Quit - <F12>" << std::endl;
}

int main()
{
	ShowUsage();

	int BHopControl = 0;
	HANDLE hBHopThread = CreateThread(NULL, NULL, BHopStart, &BHopControl, NULL, NULL);

	while (true) {
		if (!BHopControl && GetAsyncKeyState(VK_F6)) {
			std::cout << "Enabling Bunny Hop" << std::endl;
			BHopControl = 1;
		}
		else if (BHopControl && GetAsyncKeyState(VK_F7)) {
			std::cout << "Disabling Bunny Hop" << std::endl;
			BHopControl = 0;
		}
		else if (GetAsyncKeyState(VK_F12)) {
			TerminateThread(hBHopThread, 0);
			exit(0);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	return 0;
}

