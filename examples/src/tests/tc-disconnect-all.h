//this file simply contains the definition of a test case that will be included into 
//the run-tests.cpp file. I have simply copied it out to its own file which I am including back into
//the main program to clean things up a little. Dont worry about Visual Studio making it look like 
//it has errors. That is due to it not having any understanding of the context it runs in.

Tests.add("Disconnect All", [](evil::TestHarness *th,std::string& result)mutable->bool {
	
	bool failed = false;
	//one signal many slots

	//create a signal
	evil::Signal<> Asig("Asig");

	int numSlots = 20;
	//create a pile of slots and attach them to the signal
	std::vector<evil::Slot<>> slts(numSlots);
	for (int i = 0; i < numSlots; ++i) {
		Asig.add(&slts[i]);
	}

	//count the slots - should be numSlots
	if (Asig.numSlots() != numSlots) {
		result = "Signal::add";
		failed = true;
	}

	Asig.removeAll();

	//count the slots - should be none
	if (Asig.numSlots() != 0) {
		result = "Signal::removeAll";
		failed = true;
	}

	return !failed;
});