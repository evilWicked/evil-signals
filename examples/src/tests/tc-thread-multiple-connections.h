//this file simply contains the definition of a test case that will be included into 
//the run-tests.cpp file. I have simply copied it out to its own file which I am including back into
//the main program to clean things up a little. Dont worry about Visual Studio making it look like 
//it has errors. That is due to it not havingany understanding of the context it runs in.

Tests.add("Thread: Multi Connect", [](evil::TestHarness *th,std::string& result)mutable->bool {

	//NOTE This test does not test threading - only checks basic functionality.

		bool failed = false;
		//one signal many slots

		//create a signal
		evil::ThreadSignal<> Asig("Asig");

		int numSlots = 20;
		//create a pile of slots and attach them to the signal
		std::vector<evil::ThreadSlot<>> slts(numSlots);
		for (int i = 0; i < numSlots; ++i) {
			Asig.add(&slts[i]);
		}

		//count the slots - should be numSlots
		if (Asig.numSlots() != numSlots) {
			result = "ThreadSignal::add";
			failed = true;
		}
		
		//if the previous didn't fail 
		if(!failed){

			//create temporary list - we want to check are slots are still connected later
			std::list<evil::ThreadSlot<> *> tmp;

			for (int i = 0; i < numSlots; i++) {
				tmp.push_back(&slts[i]);
			}

			//remove them in random order
			while(Asig.numSlots()) {
				int N = Asig.numSlots();
				int r=rand() % N ;	

				std::list<evil::ThreadSlot<> *>::iterator it = tmp.begin();
				std::advance(it, r);
				
				Asig.remove((*it));
				tmp.remove((*it));
			}

			//count the number of slots - should be none
			if (Asig.numSlots() != 0) {
				result = "ThreadSignal::remove sig fail";
				failed = true;
			}

			if (!failed) {
				//count the connected slots - should be none
				int count = 0;
				for (int i = 0; i < numSlots; i++) {
					if (Asig.hasSlot(&slts[i]) || slts[i].signal()!=NULL) {
						count++;
					}
				}
		
				if (count != 0) {
					result = "ThreadSignal::remove slot fail";
					failed = true;
				}
			}			
		}

		return !failed;
});
