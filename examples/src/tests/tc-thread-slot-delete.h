//this file simply contains the definition of a test case that will be included into 
//the run-tests.cpp file. I have simply copied it out to its own file which I am including back into
//the main program to clean things up a little. Dont worry about Visual Studio making it look like 
//it has errors. That is due to it not havingany understanding of the context it runs in.

cTests.add("Multi Thread Slot Delete", [](std::string& result)mutable->bool {

		const int numThreads = 10;
		std::thread aThread[numThreads];
		std::vector<evil::CSlot<> *> vSlts(numThreads);
		bool failed = false;

		//create a signal
		evil::CSignal<> Asig("Asig");
		//add a number of slots to it

		for (int i = 0; i < numThreads; ++i) {
			std::string name = "signal " + std::to_string(i);
			vSlts[i] = new evil::CSlot<>(name.c_str());
		}

		//Launch a group of threads to delete them from the signal 
		for (int i = 0; i < numThreads; ++i) {

			aThread[i] = std::thread([&](int i) {
				delete vSlts[i];
			}, i);
		}


		for (int i = 0; i < numThreads; ++i) {
			aThread[i].join();
		}

		//count the slots - should be 0
		if (Asig.numSlots() != 0) {
			result = "Thread: Slot Deletion Fail";
			failed = true;
		}

		return !failed;
});