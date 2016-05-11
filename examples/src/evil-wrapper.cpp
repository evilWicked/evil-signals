
//DANGER DANGER WILL ROBINSON https://www.youtube.com/watch?v=RG0ochx16Dg 
//This file is included by each of the sample projects - changes to it will hit all samples.....

//bring in the implementation of our evil libraries

#include "evil/imp/signal.cpp"
#include "evil/imp/slot.cpp"


//it doesn't have to be done this way.  You could have one wrapper file per evil component. You could bring them
//all into an existing file (careful with namespaces or it will have a bit of an identity crisis). You choice.
//All it does is squirt evil code into your existing compilation context and you can worry about opimizations,
//locations of libraries, headers and all of that.