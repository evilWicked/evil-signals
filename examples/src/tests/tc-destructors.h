//this file simply contains the definition of a test case that will be included into 
//the run-tests.cpp file. I have simply copied it out to its own file which I am including back into
//the main program to clean things up a little. Dont worry about Visual Studio making it look like 
//it has errors. That is due to it not havingany understanding of the context it runs in.

Tests.add("Destruction", [](evil::TestHarness *th,std::string& result)mutable->bool {
			
		//create a signal
		evil::Signal<> Asig("Asig");

		//create some scope
		{
			//create a slot
			evil::Slot<> Aslt("Aslt");

			//connect them
			Asig.add(&Aslt);
			//test if we are connected
			if (!Asig.hasSlot(&Aslt) || Aslt.signal()!=&Asig) {
				result = "Signal::add";
				return false;
			}
		}//leave the scope
		

		if (Asig.numSlots()) {
			result = "Slot::~Slot";
			return false;
		}

		//got here so we passed
		return true;
});