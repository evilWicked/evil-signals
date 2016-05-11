
#ifdef _WIN32

#define EVIL_WIN
#include <vld.h> 
#include <windows.h>

#elif __APPLE__
#define EVIL_OSX
#endif

#include <iostream>


//include these as you normally would
#include "evil/signal.h"
#include "evil/slot.h"

//bring in the evil test harness
#include "test/test-harness.h"


//this is an example so we wont assume the evil namespace

int main(int argc, char* argv[])
{
	evil::CTestHarness cTest("Evil Signals Testing");

	for (int i = 0; i < 100; ++i) {

	cTest.add("FRED", [](std::string& result)mutable ->bool {
		std::string str = "AHGJHASGJHSGJ";
		int a = 0; 
		for (int i = 0; i < 100000;++i) {
			a+=(int)std::strlen(str.c_str());
		}

		std::cout << "AHGJHASGJHSGJ\n"; 
		result = "Test Result"; 
		return true; 
	});


	}

	
	cTest.run();

	//create a signal

	//create a slot

	//attach a function callback to the slot

	//create another slot 

	//attach a classcallback to the slot

	//create another slot 

	//attach a lambda callback to the slot

	//dispatch a signal

	//change the priorities reverse them

	//remove the middle (class callback)

	//dispatch a signal

	//remove the first (function callback)

	//remove the last (lambda callback)


	//create a signal that accepts a parameter list
	
	//create a slot that receives a parameter list
	
	//dispatch the signal

	//create a thread with a signal on it sends a string to any slots that fires every second.

	//create a few of these
	//create a thread that checks a list for input every 300ms if so echoes it otherwise sleeps
	//add a slot that if triggers adds a string to the list

	//connect the slots to the signal





std::cout << "Press return to exit";
std::getchar();


}
