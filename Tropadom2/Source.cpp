#include "BoxCore.h"
#include <iostream>
#include <math.h>

void main()
{
	BoxCore* core = new BoxCore();
	core->setFPSCap(600); 
	core->run();
}
