#include "BoxCore.h"
#include <iostream>
#include <string>
#include <Windows.h>

static const int MAX_WDIR_LENGTH = 100;

void main()
{
	char buffer[MAX_WDIR_LENGTH];
	GetCurrentDirectoryA(MAX_WDIR_LENGTH, buffer);
	std::string resourcePath(buffer);
	std::cout << "WDIR: " << resourcePath << std::endl;
	BoxCore* core = new BoxCore(resourcePath);
	core->setFPSCap(600);
	core->run();
}
