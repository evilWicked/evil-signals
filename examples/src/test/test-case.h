#pragma once

#include <string>
#include <functional>
#include <chrono>
#include <list>
#include <fstream>
#include <vector>

//implementation bought in at the bottom of the file

namespace evil {

class CTestCase {

	typedef std::chrono::high_resolution_clock Time;
	typedef std::chrono::milliseconds ms;
	typedef std::chrono::duration<float> fsec;

public:
	typedef std::function<bool(std::string&)> type_testfunc;

	const std::string mName;
	std::string mMessage;
	bool mbPass;
	bool mbRun;
	ms mmMilliSecs;

	const type_testfunc mTestFunction;

	CTestCase(const char *name,type_testfunc func);
	bool run();
	~CTestCase();
};

}

#include "imp/test-case.cpp"