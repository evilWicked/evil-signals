//this file simply contains the definition of a test case that will be included into 
//the run-tests.cpp file. I have simply copied it out to its own file which I am including back into
//the main program to clean things up a little. Dont worry about Visual Studio making it look like 
//it has errors. That is due to it not having any understanding of the context it runs in.

Tests.add("Thread Stress Modify", [](evil::TestHarness *th, std::string& result)mutable->bool {

	//NOTE This test does test threading.

	namespace m = evil::math;

	//create some signals
	const int numSignals = 5;
	std::vector<evil::ThreadSignal<int>>  vecSignals(numSignals);

	//create a collection of slots with associated objects
	const int numSlots = 100;
	std::vector<evil::ThreadSlot<int>>  vecSlots(numSlots);
	std::vector<TestClass>  vecObjects(numSlots);

	std::queue<evil::ThreadSlot<int> *>  queSlots;

	//connect the objects to the slots and push the slots onto the slot queue.
	//we will 
	for (int i = 0; i < numSlots; ++i) {
		vecSlots[i].attach_class(&vecObjects[i], &TestClass::testFunction4);
		queSlots.push(&vecSlots[i]);
	}

	const int numThreads = 7;
	
	std::mutex queueMtx;
	std::vector<evil::ThreadPool::TaskFuture<void>> v;

	for (int i = 0; i < numThreads; ++i) {

		th->outDot();

		v.push_back(

			evil::thread_pool::run([&] {
			int numTests = 10000;

			while (numTests) {
			
				if (numTests % 10000 == 0) th->outDot();

				//using m=evil::math;
				//randomly pick an action
				int action = m::rand_int(0, 30);

				switch (action) {

				case 0: {
					evil::ThreadSlot<int> *x = NULL;

					{
						//lock the slot queue and get one of the slots
						std::lock_guard<std::mutex> lock(queueMtx);
						if(queSlots.size()) {
							x = queSlots.front();
							queSlots.pop();
						}
						
					}//we are out from under the lock and DO have unique ownership of this slot

					//the Signals are not locked.

					//slots must be disconnected before adding - they will assert out otherwise.
					//Normally you would know what signal a slot is connected to but we are doing this randomly 
					//so we will call remove on all signals - this will exercise things anyway

					if(x) {
						for(int i = 0; i < numSignals; i++) {
							
							bool has_slot=vecSignals[i].hasSlot(x);
							//cant use the has_slot knowledge without locking a lock guard
							//because something else can change things before we use it - 
							//test we can call it anyway

							vecSignals[i].remove(x);
						}
					}
					
					//choose a random signal and add the slot 
					int n = m::rand_int(0, numSignals - 1);

					//alternate between addOnce and add with a random priority and random state
					if(n % 2) {
						vecSignals[n].add(x,m::rand_int(-5, 5), m::rand_bool());
					} else {
						vecSignals[n].addOnce(x, m::rand_int(-5,5),m::rand_bool());
					}
					

					{//push the slot back onto the list - need to be under the queue lock

						std::lock_guard<std::mutex> lock(queueMtx);
						queSlots.push(x);
					}

				}; break;

				case 1: {
					//choose a random signal and and if it has more than 10 slots remove all of them
					//NOTE we are not locking anything here.
					int n = m::rand_int(0, numSignals - 1);
					if(vecSignals[n].numSlots()>10) {
						vecSignals[n].removeAll();
					}
					
				}; break;


				case 2: {

					evil::ThreadSlot<int> *x = NULL;

					{
						//lock the slot queue and get one of the slots - dont pop it
						std::lock_guard<std::mutex> lock(queueMtx);
						if(queSlots.size()) {
							x = queSlots.front();
						}

					}//we are out from under the lock and DON'T have unique ownership of this slot


					if(x != NULL) {
						//randomly change its state
						x->setActive(m::rand_bool());
					}
					

				}; break;

				case 3: {
					//see if we can get a slots signal - its only stable the instant we grab it unless under a lock
					//but we are testing the access not its ownership so we wont lock.

					evil::ThreadSlot<int> *x = NULL;

					{
						//lock the slot queue and get one of the slots - dont pop it
						std::lock_guard<std::mutex> lock(queueMtx);
						if(queSlots.size()) {
							x = queSlots.front();
						}

					}//we are out from under the lock and DON'T have unique ownership of this slot


					//can we call the api on this slot without locking
					if(x != NULL) {
						evil::ThreadSignal<int> *sig=(evil::ThreadSignal<int> *)x->signal();
						bool bactive = x->isActive();
						bool bfire_once = x->isFireOnce();
					}


				}; break;


				default: {
					//pick a signal and trigger a dispatch
					int n = m::rand_int(0, numSignals - 1);
					evil::ThreadSignal<int> *sig = &vecSignals[n];
					sig->dispatch(1);

				}; break;

				}

				numTests--;
			}
		}));
	}

	for (auto& item : v) {
		item.get();
	}

	return true;

});