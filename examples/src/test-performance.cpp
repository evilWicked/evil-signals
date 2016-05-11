
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
#include "evil/signal.h"
#include "evil/slot.h"



//this is an example so we wont assume the evil namespace



int main(int argc, char* argv[])
{





std::cout << "Press return to exit";
std::getchar();

#ifdef _WIN32
	int _CrtDumpMemoryLeaks(void);
#endif

}
