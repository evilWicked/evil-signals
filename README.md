evil signals {#mainpage}
============

About
-----


Installation
------------
evil signals is installed by simply the source similar to a header only implementation. The simplest way to do this is simply to copy the whole `evil-signals/`  directory somewhere on your include path. Alternatively point your include path to wherever you have cloned the repo.

include header files as usual using 

    #include "evil/signals.h"
    #include "evil/slots.h"

To bring in the implementation files you can follow any of the approaches below. 

1. Either open a new .cpp file in your source somehere and simply include the implementations. You can open a new file for each .cpp file or bring them all into a single one. I use the latter.

		#include "evil/imp/signals.cpp"
		#include "evil/imp/slots.cpp"

2. Alternatively you can place the above implementation includes into an existing source file that contains your own code. If you take this approach be careful of namespaces. The evil classes reside inside the evil namespace and will have an identity crisis if you mix things up.  Your system is likely to already have a signals.h file, so it may help to keep paths as `"evil/signals.h"` rather than `"signals.h"`

3. If you want you can copy the source files out of evil/imp/ into your own code and use them like any other source.

CMake?  CMake is used in examples but CMake requires configuration and testing for each platform, so no.

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


Multithreading
--------------

Multithreading. A can of worms...

There is a long discussion about it in the documentation on the page Design Ideas: Signals which generally comes to the
conclusion that multithreading with signals and slots is not for the feint hearted.  With that said evil-signals supports multi threading.

To cover any situation where you have mutiple threads interacting with a single signal you need to enable lock guards on the signal.

    signal.useLockGuards(true); 

It defaults to false. 

To cover any situation where you have mutiple threads interacting with a single slot you need to enable lock guards on the slot.

    slot.useLockGuards(true); 
	
It also defaults to false.

The fiollowing scenarios all fit under the umbrella of multithreading.

### Scenarios 
1. A single signal on a single thread calling different slots on multiple threads.
2. A single signal on multiple threads talking to the same slot.
3. A single signal on a single thread talking to a single slot on multiple threads.
4. A single signal on multiple threads talking to a single slot on multiple threads. 
5. Changing the state of a signal from several threads (not receiving a signal - which doesn't change state).
6. Changing the state a slot from several threads (not sending a signal - which doesn't change state).

The only one I really recommend is the first. A single signal on a single thread calling different slots on multiple threads.


Releases
--------


License
-------

BSD like.