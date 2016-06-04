//this file simply contains the definition of a test case that will be included into 
//the run-tests.cpp file. I have simply copied it out to its own file which I am including back into
//the main program to clean things up a little. Dont worry about Visual Studio making it look like 
//it has errors. That is due to it not having any understanding of the context it runs in.


Tests.add("Thread: Basic Ops", [](evil::TestHarness *th,std::string& result)mutable->bool {
	
	//NOTE This test does not test threading - only checks basic functionality.

		//create a signal
		evil::ThreadSignal<> Asig("Asig");
		//create a slot
		evil::ThreadSlot<> Aslt("Aslt");

		//connect them
		Asig.add(&Aslt);
		//test if we are connected
		if (!Asig.hasSlot(&Aslt) || Aslt.signal()==0) {
			result = "ThreadSignal::add";
			return false;
		}
		//disconnect them
		Asig.remove(&Aslt);
		//test we are disconnected
		if (Asig.hasSlot(&Aslt) || Aslt.signal()!=0) {
			result = "ThreadSignal::remove";
			return false;
		}

		//got here so we passed
		return true;
});