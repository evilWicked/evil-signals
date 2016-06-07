//this file simply contains the definition of a test case that will be included into 
//the run-tests.cpp file. I have simply copied it out to its own file which I am including back into
//the main program to clean things up a little. Dont worry about Visual Studio making it look like 
//it has errors. That is due to it not having any understanding of the context it runs in.
Tests.add("Thread: Stress Dispatch", [](evil::TestHarness *th,std::string& result)mutable->bool {

	//NOTE This test does test threading.

		bool failed = false;
		const int numSlots = 20;

		//create a signal
		evil::ThreadSignal<int> Asig("Asig");//,

		//create a pile of slots and attach them to the signal
		std::vector<evil::ThreadSlot<int>> aSlots(numSlots);

		//create a test class for every slot
		std::vector<TestClass> aTests(numSlots);

		for (int i = 0; i < numSlots; ++i) {
			//attach the callback to the slot
			aSlots[i].attach_class(&aTests[i], &TestClass::testFunction4);
			//attach the slots to the signal
			Asig.add(&aSlots[i]);
		}

		const int numThreads = 7;
		const int numSignals = 10000;
		
		std::vector<evil::ThreadPool::TaskFuture<void>> v;

		for (int i = 0; i < numThreads; ++i) {
			th->outDot();
			v.push_back(			
				evil::thread_pool::run([&]{
					for (auto j = 0; j < numSignals; ++j) {
						Asig.dispatch(1);
						//std::this_thread::sleep_for(std::chrono::milliseconds(10));
					}
				})
			);
		}

		for (auto& item : v) {
			item.get();
		}

		//add up the value on each slot, should equal to numSignals*numSlots*numThreads
		int total = 0;
		for (int i = 0; i < numSlots; ++i) {
			total += aTests[i].ival;
		}

		if (total != numSignals*numSlots*numThreads) {
			result = "Failed: Thread Stress Dispatch";
			failed = true;
		}

		return !failed;

});
