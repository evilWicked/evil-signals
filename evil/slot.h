#pragma once

#include "imp/signal-base.h"
#include "imp/slot-base.h"

namespace evil {

	/**	
	See the discussion in [Design Ideas: Signals](@ref designsignals)
	*/

	class CSlot : public CSlotBase {

	public:
		CSlot(const char* name = NO_NAME):CSlotBase(name){};
	};
};
