//this file simply contains the definition of a test case that will be included into 
//the run-tests.cpp file. I have simply copied it out to its own file which I am including back into
//the main program to clean things up a little. Dont worry about Visual Studio making it look like 
//it has errors. That is due to it not havingany understanding of the context it runs in.

cTests.add("Multiple Connections", [](std::string& result)mutable->bool {

		bool failed = false;
		//one signal many slots

		//create a signal
		evil::CSignal<> Asig("Asig");

		int numSlots = 20;
		//create a pile of slots and attach them to the signal
		std::list<evil::CSlot<> *> slts;
		for (int i = 0; i < numSlots; ++i) {
			evil::CSlot<> *x = new evil::CSlot<>();
			slts.push_back(x);
			Asig.add(x);
		}

		//count the slots - should be numSlots
		if (Asig.numSlots() != numSlots) {
			result = "Signal::add";
			failed = true;
		}
		
		//if the previous didn't fail 
		if(!failed){

			//create temporary list - we want to check are slots are still connected later
			std::list<evil::CSlot<> *> tmp = slts;
			//remove them in random order
			while(Asig.numSlots()) {
				int N = Asig.numSlots();
				int r=rand() % N ;	

				//a hack - but its a test s- not worried about performance
				std::list<evil::CSlot<> *>::iterator it = tmp.begin();
				for (int i = 0; i < r; ++i) {
					it++;
				}
				Asig.remove((*it));
				tmp.remove((*it));
			}

			//count the number of slots - should be none
			if (Asig.numSlots() != 0) {
				result = "Signal::remove sig fail";
				failed = true;
			}

			if (!failed) {
				//count the connected slots - should be none
				int count = 0;
				std::list<evil::CSlot<> *>::iterator it;
				for (it = slts.begin(); it != slts.end(); it++) {
					if ((*it)->isConnected()) {
						count++;
					}
				}

				if (count != 0) {
					result = "Signal::remove slot fail";
					failed = true;
				}
			}			
		}

		//clean up
		std::list<evil::CSlot<> *>::iterator it;
		for (it = slts.begin(); it != slts.end(); it++) {
			delete *it;
		}

		return !failed;
});
