#pragma once

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

//bring in the wrapper which includes all the implementation files
#include "evil/evil-wrapper.cpp"

//bring in the evil test harness - this includes its own implementations
#include "test/test-harness.h"


//this is an example so we wont assume the evil namespace - lets do it as a user would do it

int main(int argc, char* argv[])
{
	//create some inner scope so that the leak detector does not report top level objects
	//as leaking. It will force the destruction before main exists - not needed for normal code.
	{

		evil::CTestHarness cTests("Evil Signals Testing");

		cTests.add("Basic Create Remove", [](std::string& result)mutable->bool {
			//create some scope to force destruction
			{
				//create a signal
				evil::CSignal Asig("Asig");
				//create a slot
				evil::CSlot Aslt("Aslt");

				//connect them
				Asig.add(&Aslt);
				//test if we are connected
				if (!Aslt.isConnected()) {
					result = "Signal::add";
					return false;
				}
				//disconnect them
				Asig.remove(&Aslt);
				//test we are disconnected
				if (Aslt.isConnected()) {
					result = "Signal::remove";
					return false;
				}

				//connect them from the slot side
				Aslt.connect(&Asig);
				//test if we are connected
				if (!Aslt.isConnected()) {
					result = "Slot::connect";
					return false;
				}

				//disconnect them
				Aslt.disconnect();
				//test we are disconnected
				if (Aslt.isConnected()) {
					result = "Slot::disconnect";
					return false;
				}

				//got here so we passed
				return true;
			}
		});


		cTests.add("Destruction Test", [](std::string& result)mutable->bool {
			
			//create a signal
			evil::CSignal Asig("Asig");

			//create some scope
			{
				//create a slot
				evil::CSlot Aslt("Aslt");

				//connect them
				Asig.add(&Aslt);
				//test if we are connected
				if (!Aslt.isConnected()) {
					result = "Signal::add";
					return false;
				}
			}//leave the scope
			

			if (Asig.numSlots()) {
				result = "Slot::~Slot";
				return false;
			}

			//create a slot
			evil::CSlot Aslt("Aslt");

			//create some scope
			{
				//create a signal
				evil::CSignal Asig("Asig");

				//connect them
				Asig.add(&Aslt);
				//test if we are connected
				if (!Aslt.isConnected()) {
					result = "Signal::add";
					return false;
				}
			}//leave the scope
			
			if (Aslt.isConnected()) {
				result = "Signal::~Signal";
				return false;
			}

			//got here so we passed
			return true;
		});



		cTests.add("Multiple Connections", [](std::string& result)mutable->bool {

			bool failed = false;
			//one signal many slots

			//create a signal
			evil::CSignal Asig("Asig");

			int numSlots = 20;
			//create a pile of slots and attach them to the signal
			std::list<evil::CSlot *> slts;
			for (int i = 0; i < numSlots; ++i) {
				evil::CSlot *x = new evil::CSlot();
				slts.push_back(x);
				Asig.add(x);
			}

			//count the slots - should be numSlots
			if (Asig.numSlots() != numSlots) {
				result = "Signal::add";
				failed = true;
			}
			
			//if the previous didn't fail 
			if(!failed){

				//create temporary list - we want to check are slots are still connected later
				std::list<evil::CSlot *> tmp = slts;
				//remove them in random order
				while(Asig.numSlots()) {
					int N = Asig.numSlots();
					int r=rand() % N ;	

					//a hack - but its a test s- not worried about performance
					std::list<evil::CSlot *>::iterator it = tmp.begin();
					for (int i = 0; i < r; ++i) {
						it++;
					}
					Asig.remove((*it));
					tmp.remove((*it));
				}

				//count the number of slots - should be none
				if (Asig.numSlots() != 0) {
					result = "Signal::remove sig fail";
					failed = true;
				}

				if (!failed) {
					//count the connected slots - should be none
					int count = 0;
					std::list<evil::CSlot *>::iterator it;
					for (it = slts.begin(); it != slts.end(); it++) {
						if ((*it)->isConnected()) {
							count++;
						}
					}

					if (count != 0) {
						result = "Signal::remove slot fail";
						failed = true;
					}
				}
				
			}

			//clean up
			std::list<evil::CSlot *>::iterator it;
			for (it = slts.begin(); it != slts.end(); it++) {
				delete *it;
			}

			return !failed;
		});



		cTests.add("Multi Threads to Signal", [](std::string& result)mutable->bool {

			//have multiple threads adding removing and sorting signals. We are simply 
			//going to stress test this by randomly adding and removing
			return true;
		});














		cTests.run();
	}



















	/*
	cTest.add("Multi Create/Remove", [](std::string& result)mutable->bool{

	});

	cTest.add("Single to Threads", [](std::string& result)mutable->bool {
	});

	cTest.add("Disconnection", [](std::string& result)mutable->bool {
	});

	cTest.add("Multi Connect", [](std::string& result)mutable->bool {
	});

	cTest.add("Multi Disconnect", [](std::string& result)mutable->bool {
	});
	*/
	

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
