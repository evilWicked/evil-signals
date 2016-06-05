evil signals
============

About
-----

evil signals provides a simple lightweight C++ threadsafe signals and slots implementation. It contains both threadsafe and 
non threadsafe versions. The normal (non threadsafe but faster) version consists of evil::Signal<...Args> paired with evil::Slot<...Args> .
The threadsafe pair evil::ThreadSignal<...Args> paired with evil::ThreadSlot<...Args> utilises a Read Write Lock that enables parallel dispatches together with sequential modifications. 

It is intended to be used as a source only distribution relying upon C++11 std libraries.

Documentation
-------------

The Doxygen documentation can be found on github at http://evilwicked.github.io/evil-signals/index.html.  Alternatively you can build
your own using the doxyfile in the docs/ directory.

Installation
------------
evil signals is installed by simply including the source. The simplest way to do this is to copy the whole `evil-signals/`  directory somewhere into your include path. Alternatively point your include path to wherever you have cloned the repo.

include header files as usual using 

    #include "evil/signals/signals.h"
    #include "evil/signals/slots.h"
	
To bring in the implementation (*.cpp) files simply   

    #include "evil/signals/wrapper.cpp"

into a source file somewhere. You can give it its own file or include it into an existing files. Whatever
feels best.  Take care to keep the directory structure under evil/ intact. This contains other code that is used by either signals itself or the examples.


Why not use CMake?  CMake is used in examples but not for the main code. CMake would require configuration and testing for each platform which if it doesn't work would generate issues. So no.

Compiler Options
----------------
Nothing special. Just treat it as your own code and optimise to suit your own needs.
 
Dependencies
------------
None.

Built using C++11 std libraries.

Examples
--------
The examples code can be built using CMake.  But alas I don't have a Mac or any of the plethora of Linux boxes that exist out there so they have only been run on Windows.

Run CMake as usual. Point the CMake source dir to the `evil-signals/examples/` and the CMake build dir to `evil-signals/examples/build/` which is where your `evil-signals-examples.sln` will end up as well as a pile of CMake inspired debris.


Usage
-----
	
Usage is fairly simple. Both the signal and the slot are templated to handle different arguments being passed between the signal
and the slot when a signal is dispatched.

	evil::Signal<...Args>
	evil::Slot<...Args>
		
As an example if we want to use callbacks of the form

	void foo(int i){...};

###Create a signal slot pair
	
We can create a signal and slot that would enable an integer to be passed from the slot to the signal with:

	evil::Signal<int> sig;
	evil::Slot<int> slot;

####Threadsafe
	
The threadsafe versions are created using.
	
	evil::ThreadSignal<...Args> sig;
	evil::ThreadSlot<...Args> slot;

The only restriction is that callbacks need to have a void return.  The signal has no way of
doing anything with a return value anyway.

###Attach a callback to the slot.

To attach a callback to the slot you can use a standard function, a lambda or a class member.
Normal functions and lambdas use "attach" where classes that need an instance of the class to be bound
use "attach_class". So if foo is a function

	void foo(){}

you would use;

	slot.attach(foo);


If C is a class with an instance c you would use

	class C{
	public:
	void foo(){}
	}

	C c;

	slot.attach_class(&c,foo);

###Connect the slot to the signal
	
To connect the slot to the signal you use add from the signal.

	sig.add(&slot)

similarly to disconnect them you use remove.

	sig.remove(&slot)

The add signature allows an optional integer priority.  Higher priority lots fire earlier. You
can also set a slot to fire once and automatically be removed.  This can be useful when booting
into code where you are waiting for an event that will occur only once.

	void add(SlotBase *slot, int priority = 0, bool active = true);

###Dispatch a signal.
	
To dispatch a signal you simply call dispatch passing any arguments to it, In the above

	sig.dispatch(1);
	
would pass 1 to each attached slot when dispatch is called.

####Passing by reference.  

You can pass objects by reference to slots. This is not a constant reference, so be careful with this. 
I had considered making it const but that would restrict what you do. In the choice between you being 
careful or you not being allowed to do something - I take the Javascript view that its your code - do 
what you want. This means that if you define

	evil::Signal<int&> sig;
	evil::Slot<int&> slot1;
	evil::Slot<int&> slot2;

	void foo(int &i){i++;}
	sig.add(&slot1);
	sig.add(&slot2);

	int i=0;
	sig.dispatch(i);

i would now be now equal to 2 becuase it would be incremented with every call of the slot by the signal.

Slots can be deactivated and reactivated.

	slot.setActive(false);
	slot.setActive(true);

Multithreading
--------------

There are two versions of the both the signals and the slots that are derived from the same underlying classes.  The normal
versions  evil::Signal and evil::Slot are not thread safe.   The threadsafe classes are evil::ThreadSignal and evil::ThreadSlot
These are implemented using read write locks enabling parallel dispatch of signals alongside mutex locked changes to the signal
and slot.

Releases
--------

1.0.0  Initial release.


License
-------

BSD like.