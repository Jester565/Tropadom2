#include "BoxCore.h"
#include <iostream>
#include <string>
#include <Windows.h>

static const int MAX_WDIR_LENGTH = 100;

void main()
{
	BoxCore* core = new BoxCore("");
	core->setFPSCap(600);
	core->run();
}
