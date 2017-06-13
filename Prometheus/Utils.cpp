#include "stdafx.h"
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
	std::this_thread::sleep_for(std::chrono::milliseconds(1500));
	exit(-1);
}
