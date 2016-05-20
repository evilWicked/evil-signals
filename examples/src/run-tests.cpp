#pragma once

#ifdef _WIN32

#define EVIL_WIN
#include <vld.h> 
#include <windows.h>

#elif __APPLE__
#define EVIL_OSX
#endif

#include <iostream>
#include <vector>
#include <string>

//include these as you normally would
#include "evil/signals/signal.h"
#include "evil/signals/slot.h"

//bring in the signals wrapper which includes the signal implementation files
#include "evil/signals/evil-wrapper.cpp"
//you could include this file in your sorce - if so dont also include it or 
//you will end up with duplicate symbol errors

//bring in the evil test harness 
#include "evil/test-harness/test-harness.h"

//bring in the test wrapper which includes the test harness implementation files
#include "evil/test-harness/evil-wrapper.cpp"

//some test objects we will use to test things are working

class CTestClass {
public:
	int ival;
	double dval;
	std::string strval;

	CTestClass() {
		ival = 0;
		dval = 0.0;
		strval = "";
	}

	void testFunction1(int i) { 
		ival += i; 
	}

	void testFunction2(double x) { 
		dval += x; 
	}

	void testFunction3(int i, double x,std::string &str) {
		ival += i;
		dval += x;
		strval = "got here";
	}
};

int iTestVal=0;
double dTestVal=0.0;

void testFunction1(int i) {
	iTestVal += i;
}

void testFunction2(double x) {
	dTestVal += x;
}

void testFunction3(int i, double x, std::string &str) {
	iTestVal += i;
	dTestVal += x;
	str = "got here as well";
}



//this is an example so we wont assume the evil namespace - lets do it as a user would do it

int main(int argc, char* argv[])
{
	//create some inner scope so that the leak detector does not report top level objects
	//as leaking. It will force the destruction before main exists - not needed for normal code.
	{

		evil::CTestHarness cTests("Evil Signals Testing");

		//include our test cases - they are just lambdas added by
		//cTests.add("some name", [](std::string& result)mutable->bool {do something return pass/fail})

		#include "tests/tc-basic-create.h"
		#include "tests/tc-destructors.h"
		#include "tests/tc-multiple-connections.h"
		#include "tests/tc-thread-sig-create.h"
		#include "tests/tc-thread-slot-add.h"
		#include "tests/tc-thread-slot-delete.h"
		#include "tests/tc-basic-trigger-callback.h"
		#include "tests/tc-class-trigger-callback.h"
		#include "tests/tc-single-stress-dispatch.h"
		#include "tests/tc-multi-stress-dispatch.h"
		

		cTests.run();
	}



std::cout << "Press return to exit";
std::getchar();


}
