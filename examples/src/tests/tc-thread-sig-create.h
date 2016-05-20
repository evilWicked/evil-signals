//this file simply contains the definition of a test case that will be included into 
//the run-tests.cpp file. I have simply copied it out to its own file which I am including back into
//the main program to clean things up a little. Dont worry about Visual Studio making it look like 
//it has errors. That is due to it not havingany understanding of the context it runs in.

cTests.add("Multi Thread Signal Create", [](std::string& result)mutable->bool {

		const int numThreads=10;
		std::thread aThread[numThreads];
		std::vector<evil::CSignal<> *> vSigs(numThreads);
		bool failed = false;

		//simply create a numThreads signals using different threads

		//Launch a group of threads
		for (int i = 0; i < numThreads; ++i) {
			
			aThread[i] = std::thread([&](int i) {
				std::string name = "signal " + std::to_string(i);
				vSigs[i] = new evil::CSignal<>(name.c_str());
			},i);
		}


		for (int i = 0; i < numThreads; ++i) {
			aThread[i].join();
		}

		if (vSigs.size() != numThreads) {
			result = "Thread: Signal Creation Fail";
			failed = true;
		}

		for (int i = 0; i < numThreads; ++i) {
			delete vSigs[i];
		}


		return !failed;
});


