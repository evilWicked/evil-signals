//this file simply contains the definition of a test case that will be included into 
//the run-tests.cpp file. I have simply copied it out to its own file which I am including back into
//the main program to clean things up a little. Dont worry about Visual Studio making it look like 
//it has errors. That is due to it not havingany understanding of the context it runs in.

cTests.add("Multi Thread Slot Add", [](std::string& result)mutable->bool {

		const int numThreads = 10;
		std::thread aThread[numThreads];
		std::vector<evil::CSlot<> *> vSlts(numThreads);
		bool failed = false;

		//create a signal
		evil::CSignal<> Asig("Asig");

		//Launch a group of threads to create slots and add them to the signal
		for (int i = 0; i < numThreads; ++i) {

			aThread[i] = std::thread([&](int i) {
				std::string name = "signal " + std::to_string(i);
				vSlts[i] = new evil::CSlot<>(name.c_str());
				Asig.add(vSlts[i]);
			}, i);
		}


		for (int i = 0; i < numThreads; ++i) {
			aThread[i].join();
		}

		//count the slots - should be numThreads
		if (Asig.numSlots() != numThreads) {
			result = "Thread: Slot Creation Fail";
			failed = true;
		}

		for (int i = 0; i < numThreads; ++i) {
			delete vSlts[i];
		}

		return !failed;
});

