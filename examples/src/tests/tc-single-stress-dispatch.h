//this file simply contains the definition of a test case that will be included into 
//the run-tests.cpp file. I have simply copied it out to its own file which I am including back into
//the main program to clean things up a little. Dont worry about Visual Studio making it look like 
//it has errors. That is due to it not havingany understanding of the context it runs in.

cTests.add("Single Stress Dispatch", [](std::string& result)mutable->bool {

		bool failed = false;
		const int numSlots = 20;

		//create a signal
		evil::CSignal<int> Asig("Asig");//,

		//create a pile of slots and attach them to the signal
		evil::CSlot<int> aSlots[numSlots];

		//create a test class for every slot
		CTestClass aTests[numSlots];

		for (int i = 0; i < numSlots; ++i) {

			//attach the callback to the slot
			aSlots[i].attach_class(&aTests[i], &CTestClass::testFunction1);
			//attach the slots to the signal
			Asig.add(&aSlots[i]);

		}

		//dispatch a lot of signals
		const int numSignals = 1000;
		for (int i = 0; i < numSignals; ++i) {
			Asig.dispatch(1);
		}

		//add up the value on each slot, should equal numSignals*numSlots
		int total = 0;
		for (int i = 0; i < numSlots; ++i) {
			total += aTests[i].ival;
		}


		if (total != numSignals*numSlots) {
			result = "Failed: Single Dispatch";
			failed = true;
		}

		return !failed;

});
