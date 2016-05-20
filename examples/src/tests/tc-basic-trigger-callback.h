//this file simply contains the definition of a test case that will be included into 
//the run-tests.cpp file. I have simply copied it out to its own file which I am including back into
//the main program to clean things up a little. Dont worry about Visual Studio making it look like 
//it has errors. That is due to it not havingany understanding of the context it runs in.

cTests.add("Basic Trigger Callback", [](std::string& result)mutable->bool {

		bool failed = false;
		//create a signal
		evil::CSignal<int, double, std::string&> Asig("Asig");
		//create a slot
		evil::CSlot<int, double, std::string&> Aslt("Aslt");

		//add a callback to the slot
		Aslt.attach(testFunction3);

		//connect the slot to the signal
		Asig.add(&Aslt);
			
		std::string str = "Fred";
		//trigger the signal
		Asig.dispatch(10, 3.3, str);

		if (iTestVal != 10) {
			failed = true;
			result = "Trigger: Callback Fail";
		}

		return !failed;
			
});