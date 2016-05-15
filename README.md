evil signals
============

About
-----


Installation
------------
evil signals is installed by simply including the source similar to a header only implementation. The simplest way to do this is to copy the whole `evil-signals/`  directory somewhere into your include path. Alternatively point your include path to wherever you have cloned the repo.

include header files as usual using 

    #include "evil/signals.h"
    #include "evil/slots.h"

Both classes are templated so thats all you need.  Take care to keep the implementation directory imp/ that 
is found under the header files together with them as it contains teh implementation required for the template.


Why not use CMake?  CMake is used in examples but for the main code. CMake would require configuration and testing for each platform which if it doesn't work would generate issues. So no.

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

There is a long discussion about it in the documentation on the page Design Ideas: Signals which generally comes to the conclusion that multithreading with signals and slots is not for the feint hearted.  With that said evil-signals supports multi threading.




Releases
--------


License
-------

BSD like.