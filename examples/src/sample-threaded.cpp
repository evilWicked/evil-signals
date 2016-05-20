
#ifdef _WIN32
#define EVIL_WIN

#define _CRTDBG_MAP_ALLOC
//set up crt memory leak checking
#include <stdlib.h>
#include <crtdbg.h>
#include <windows.h>

#elif __APPLE__
#define EVIL_OSX
#endif

#include <iostream>

//include these as you normally would
#include "evil/signals/signal.h"
#include "evil/signals/slot.h"

//bring in the signals wrapper which includes all the implementation files
#include "evil/signals/evil-wrapper.cpp"

//bring in the evil test harness - this includes its own implementations
#include "evil/test/test-harness.h"

//bring in the test wrapper which includes all the implementation files
#include "evil/test/evil-wrapper.cpp"

//some test objects we will use to test things are working

//this is an example so we wont assume the evil namespace



int main(int argc, char* argv[])
{





std::cout << "Press return to exit";
std::getchar();

#ifdef _WIN32
	int _CrtDumpMemoryLeaks(void);
#endif

}
