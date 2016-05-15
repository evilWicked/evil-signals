#pragma once

#include <atomic>
#include <functional>
#include <mutex>

//#include "imp/signal-base.h"
//#include "imp/slot-base.h"

#define NO_NAME "NO NAME"

//template implementation included by the include at the bottom of this file.

namespace evil {

	/**
	
	See the discussion in [Design Ideas: Signals](@ref designsignals)

	Have drawn upon the basic outline of a signal defined in http://simmesimme.github.io/tutorials/2015/09/20/signal-slot/
	As well as some info on function pointer usage http://www.cplusplus.com/forum/general/159730/
	*/

	class CSlot : public CSlotBase {

	public:
		CSlot(const char* name = NO_NAME);
	};
};

//#include "imp/slot.cpp"