//this file simply contains the definition of a test case that will be included into 
//the run-tests.cpp file. I have simply copied it out to its own file which I am including back into
//the main program to clean things up a little. Dont worry about Visual Studio making it look like 
//it has errors. That is due to it not havingany understanding of the context it runs in.

cTests.add("Destruction Test", [](std::string& result)mutable->bool {
			
		//create a signal
		evil::CSignal<> Asig("Asig");

		//create some scope
		{
			//create a slot
			evil::CSlot<> Aslt("Aslt");

			//connect them
			Asig.add(&Aslt);
			//test if we are connected
			if (!Aslt.isConnected()) {
				result = "Signal::add";
				return false;
			}
		}//leave the scope
		

		if (Asig.numSlots()) {
			result = "Slot::~Slot";
			return false;
		}

		//create a slot
		evil::CSlot<> Aslt("Aslt");

		//create some scope
		{
			//create a signal
			evil::CSignal<> Asig("Asig");

			//connect them
			Asig.add(&Aslt);
			//test if we are connected
			if (!Aslt.isConnected()) {
				result = "Signal::add";
				return false;
			}
		}//leave the scope
		
		if (Aslt.isConnected()) {
			result = "Signal::~Signal";
			return false;
		}

		//got here so we passed
		return true;
});