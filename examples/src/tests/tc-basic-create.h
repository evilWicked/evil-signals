//this file simply contains the definition of a test case that will be included into 
//the run-tests.cpp file. I have simply copied it out to its own file which I am including back into
//the main program to clean things up a little. Dont worry about Visual Studio making it look like 
//it has errors. That is due to it not havingany understanding of the context it runs in.


cTests.add("Basic Create Remove", [](std::string& result)mutable->bool {
	
		//create a signal
		evil::CSignal<> Asig("Asig");
		//create a slot
		evil::CSlot<> Aslt("Aslt");

		//connect them
		Asig.add(&Aslt);
		//test if we are connected
		if (!Aslt.isConnected()) {
			result = "Signal::add";
			return false;
		}
		//disconnect them
		Asig.remove(&Aslt);
		//test we are disconnected
		if (Aslt.isConnected()) {
			result = "Signal::remove";
			return false;
		}

		//connect them from the slot side
		Aslt.connect(&Asig);
		//test if we are connected
		if (!Aslt.isConnected()) {
			result = "Slot::connect";
			return false;
		}

		//disconnect them
		Aslt.disconnect();
		//test we are disconnected
		if (Aslt.isConnected()) {
			result = "Slot::disconnect";
			return false;
		}

		//got here so we passed
		return true;
});