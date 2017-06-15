// Prometheus.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MemoryManager.h"
#include "Utils.h"

#include <ctime>
#include <iostream>
#include <thread>

#include "BunnyHop.h"
#include "Radar.h"

void ShowUsage() {
	std::cout << "Prometheus v<1.00> by ur0" << std::endl;
	std::cout << "Usage:" << std::endl;
	std::cout << "Toggle Bunny Hop - <F6>" << std::endl;
	std::cout << "Toggle Radar Hack - <F7>" << std::endl;
	std::cout << "Restart all threads - <F11>" << std::endl;
	std::cout << "Quit - <F12>" << std::endl;
}

int main()
{
	ShowUsage();

	int BHopControl = 0;
	HANDLE hBHopThread = CreateThread(NULL, NULL, BHopStart, &BHopControl, NULL, NULL);

	int RadarHackControl = 0;
	HANDLE hRadarThread = CreateThread(NULL, NULL, StartRadar, &RadarHackControl, NULL, NULL);

	while (true) {
		if (GetAsyncKeyState(VK_F6)) {
			if (BHopControl)
				std::cout << "Disabling Bunny Hop..." << std::endl;
			else
				std::cout << "Enabling Bunny Hop..." << std::endl;
			BHopControl = !BHopControl;
		}
		else if (GetAsyncKeyState(VK_F7)) {
			if (RadarHackControl)
				std::cout << "Disabling Radar Hack..." << std::endl;
			else
				std::cout << "Enabling Radar Hack..." << std::endl;
			RadarHackControl = !RadarHackControl;
		}
		else if (GetAsyncKeyState(VK_F11)) {
			TerminateThread(hBHopThread, 0);
			TerminateThread(hRadarThread, 0);
			BHopControl = 0;
			RadarHackControl = 0;
			hBHopThread = CreateThread(NULL, NULL, BHopStart, &BHopControl, NULL, NULL);
			hRadarThread = CreateThread(NULL, NULL, StartRadar, &RadarHackControl, NULL, NULL);
			std::cout << "Restarted all threads..." << std::endl;
		}
		else if (GetAsyncKeyState(VK_F12)) {
			TerminateThread(hBHopThread, 0);
			TerminateThread(hRadarThread, 0);
			exit(0);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	return 0;
}
