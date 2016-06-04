//this file simply contains the definition of a test case that will be included into 
//the run-tests.cpp file. I have simply copied it out to its own file which I am including back into
//the main program to clean things up a little. Dont worry about Visual Studio making it look like 
//it has errors. That is due to it not having any understanding of the context it runs in.


Tests.add("Deactivate", [](evil::TestHarness *th,std::string& result)mutable->bool {
	
	bool failed = false;
	const int numSlots = 20;

	//create a signal
	evil::Signal<int> Asig("Asig");//,

									//create a pile of slots and attach them to the signal
	std::vector<evil::Slot<int>> aSlots(numSlots);

	//create a test class for every slot
	std::vector<TestClass> aTests(numSlots);

	for (int i = 0; i < numSlots; ++i) {
		//attach the callback to the slot
		aSlots[i].attach_class(&aTests[i], &TestClass::testFunction1);
		//attach the slots to the signal
		Asig.add(&aSlots[i]);
	}

	int count = 0;
	for (int i = 0; i < numSlots; ++i) {
		
		if (i % 2) {
			aSlots[i].setActive(true);
			count++;
		}else{
			aSlots[i].setActive(false);
		}
	}

	//dispatch a signal
	Asig.dispatch(1);

	//add up the value on each slot, should equal count
	int total = 0;
	for (int i = 0; i < numSlots; ++i) {
		total += aTests[i].ival;
	}

	if (total != count) {
		result = "Failed: Single Deactivate";
		failed = true;
	}

	return !failed;

});