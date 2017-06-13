#include "stdafx.h"
#include <Windows.h>
#include <ctime>
#include <iostream>
#include <thread>
#include "Utils.h"


Utils::Utils()
{
}


Utils::~Utils()
{
}

void Utils::ErrorAndExit(const std::string strMessage)
{
	std::cout << strMessage << std::endl;
	printf("Last error: 0x%x\n", GetLastError());
	std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	exit(-1);
}
