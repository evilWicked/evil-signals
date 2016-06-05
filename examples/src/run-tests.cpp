#pragma once

#ifdef _WIN32

#define EVIL_WIN

//visual leak detector. 
//If you enable this note that visual leak detector sometimes reports some false
//positives due to top level objects not being deleted after the leak detection code 
//#include <vld.h> 

#include <windows.h>

#elif __APPLE__
#define EVIL_OSX
#endif

#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <queue>

//include these as you normally would
#include "evil/signals/signal.h"
#include "evil/signals/slot.h"

//we will test both the single and the threaded versions 
//so include these as well
#include "evil/signals/thread-signal.h"
#include "evil/signals/thread-slot.h"

//bring in the signals wrapper which includes the signal implementation files
#include "evil/signals/evil-wrapper.cpp"
//you could include the wrapper in your source - if you do dont also include it or 
//you will end up with duplicate symbol errors


//You would normally not use these

//bring in the evil test harness 
#include "evil/test-harness/test-harness.h"

//bring in the test wrapper which includes the test harness implementation files
#include "evil/test-harness/evil-wrapper.cpp"

//we are going to use the evil thread pool - think of it as a freebee. We are using the 
//evil::ReadWriteLock in the signals library and this comes with it.
#include "evil/thread/thread-pool.h"

//the worlds most absurd math library - contains a random integer generator. 
//Will probably extend it in the future
#include "evil/math/math.h"

//some test objects we will use to test things with

class TestClass {
	std::mutex mMtx;
public:
	int ival;
	double dval;
	std::string strval;

	TestClass() {
		ival = 0;
		dval = 0.0;
		strval = "";
	}

	void testFunction1(int i) { 
	//	std::lock_guard<std::mutex> lock(mMtx);
		ival += i; 
	}

	void testFunction2(double x) { 
		std::lock_guard<std::mutex> lock(mMtx);
		dval = x; 
	}

	void testFunction3(int i, double x,std::string &str) {
	//	std::lock_guard<std::mutex> lock(mMtx);
		ival = i;
		dval = x;
		strval = "got here";
	}

	void testFunction4(int i) {
		
		ival += i;
	//	std::vector<int> vecX;
		for(int i = 0; i < 20; i++) {
			//vecX.push_back(evil::math::rand_int(-100,100));
			dval += evil::math::rand_int(-100, 100);
		}
		//std::sort(vecX.begin(), vecX.end());
		//ival = vecX[19];
	}
};


int iTestVal=0;
double dTestVal=0.0;

void testFunction1(int i) {
	iTestVal += i;
}

void testFunction2(double x) {
	dTestVal = x;
}

void testFunction3(int i, double x, std::string &str) {
	iTestVal = i;
	dTestVal = x;
	str = "got here";
}



//this is an example so we wont assume the evil namespace - lets do it as a user would do it

int main(int argc, char* argv[])
{
	
	evil::TestHarness Tests("Evil Signals Testing");

	//include our test cases - they are just lambdas added by
	//cTests.add("some name", [](evil::CTestHarness *th,std::string& result)mutable->bool 
	//							{do something return pass/fail})

	#include "tests/tc-basic-creation.h"
	#include "tests/tc-thread-basic-creation.h"
		
	#include "tests/tc-destructors.h"
	#include "tests/tc-thread-destructors.h"
		
	#include "tests/tc-multiple-connections.h"
	#include "tests/tc-thread-multiple-connections.h"
		
	#include "tests/tc-disconnect-all.h"
	#include "tests/tc-thread-disconnect-all.h"
		
	#include "tests/tc-basic-callback.h"
	#include "tests/tc-thread-basic-callback.h"
		
	#include "tests/tc-class-callback.h"
	#include "tests/tc-thread-class-callback.h"
		
	#include "tests/tc-fire-once.h"
	#include "tests/tc-thread-fire-once.h"
		
	#include "tests/tc-deactivate.h"
	#include "tests/tc-thread-deactivate.h" 

	#include "tests/tc-stress-dispatch.h"
	#include "tests/tc-thread-stress-dispatch.h"
		
	#include "tests/tc-stress-modify.h"
	#include "tests/tc-thread-stress-modify.h"
				
	Tests.run();
	

std::cout << "Press return to exit";
std::getchar();

}
