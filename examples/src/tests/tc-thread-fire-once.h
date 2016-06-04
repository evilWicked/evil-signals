//this file simply contains the definition of a test case that will be included into 
//the run-tests.cpp file. I have simply copied it out to its own file which I am including back into
//the main program to clean things up a little. Dont worry about Visual Studio making it look like 
//it has errors. That is due to it not having any understanding of the context it runs in.

Tests.add("Thread: Fire Once", [](evil::TestHarness *th, std::string& result)mutable->bool {

	bool failed = false;
	//one signal many slots

	//create a signal
	evil::ThreadSignal<int> Asig("Asig");

	int numSlots = 20;
	//create a pile of slots and attach them to the signal
	std::vector<evil::ThreadSlot<int>> slts(numSlots);
	for (int i = 0; i < numSlots; ++i) {
	
		slts[i].attach(testFunction1);

		if (i == 5) {
			Asig.addOnce(&slts[i]);
		}else {
			Asig.add(&slts[i]);
		}	
	}

	iTestVal = 0;
	//fire the signal
	Asig.dispatch(1);

	//add teh values
	if (iTestVal != numSlots) {
		result = "ThreadSignal::add";
		failed = true;
	}

	

	return !failed;
});