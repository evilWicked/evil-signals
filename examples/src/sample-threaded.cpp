
#ifdef _WIN32
#define EVIL_WIN

#define _CRTDBG_MAP_ALLOC
//set up crt memory leak checking
#include <stdlib.h>
#include <crtdbg.h>
#include <windows.h>

#elif __APPLE__
#define EVIL_OSX
#endif


//we will uses these
#include <string>
#include <iostream>


//bring in the headers for the threadsafe signals and slots 
#include "evil/signals/thread-signal.h"
#include "evil/signals/thread-slot.h"

//bring in the signals wrapper which includes all the implementation files
#include "evil/signals/evil-wrapper.cpp"


//create a test class
class TestClass {
	public:
		void member_function(int i, std::string &str) {
			str += " :append class call i=" + std::to_string(i);
		}
};

void normal_function(int i, std::string &str) {
	str += " :append function call i=" + std::to_string(i);
}


int main(int argc, char* argv[])
{

	//create a signal - lets say it takes in an int 
	//and we will do something weird and accept a string, 
	//modify it and return it
	evil::ThreadSignal<int, std::string&> sig;

	//create some slots. we will attach a class member
	//a function and a lambda

	TestClass c;

	evil::ThreadSlot<int, std::string&> slot1;
	slot1.attach_class(&c,&TestClass::member_function);

	evil::ThreadSlot<int, std::string&> slot2;
	slot2.attach(normal_function);

	evil::ThreadSlot<int, std::string&> slot3;
	slot3.attach([](int i, std::string& str) {
		str += " :append lambda i=" + std::to_string(i);
	});

	sig.add(&slot1);
	sig.add(&slot2);
	sig.add(&slot3);

	std::string s = "the original string";
	
	std::cout << "\n";
	std::cout << s << "\n";
	sig.dispatch(1,s);
	std::cout << "result after dispatch(1) \n";
	std::cout << s << "\n";
	std::cout << "\n";
	//now we will clear the slot of all signals and will use some lambdas
	//to add up a total from different threads

	sig.removeAll();

	int total=0;
	std::mutex mtx;//to protect the total

	slot1.attach([&](int i, std::string& str) {
		//we will ignore the string
		std::lock_guard<std::mutex> lock(mtx);
		total += i;	
	});

	slot2.attach([&](int i, std::string& str) {
		//we will ignore the string
		std::lock_guard<std::mutex> lock(mtx);
		total += 2*i;
	});

	slot3.attach([&](int i, std::string& str) {
		//we will ignore the string
		std::lock_guard<std::mutex> lock(mtx);
		total -= 3*i;
	});


	sig.add(&slot1);
	sig.add(&slot2);
	sig.add(&slot3);


	int num_threads = 3;
	//create some threads
	std::string str="";

	std::thread t1([&] {
		for(int i = 0; i < 100; i++) {
			sig.dispatch(i, str); 
		}
	});

	std::thread t2([&] {
		for(int i = 0; i < 100; i++) { 
			sig.dispatch(i, str); 
		}
	});

	std::thread t3([&] {
		for(int i = 0; i < 100; i++) { 
			sig.dispatch(i, str); 
		}
	});

	t1.join();
	t2.join();
	t3.join();

	std::cout << "expecting 0  Actual result: "+std::to_string(total);
	std::cout << "Press return to exit";
	std::getchar();

}
