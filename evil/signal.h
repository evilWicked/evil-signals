#pragma once

#include "imp/signal-base.h"

namespace evil {

	/**

	See the discussion in [Design Ideas: Signals](@ref designsignals)

	*/

	class CSignal :public CSignalBase {
	
	public:
		///Constructor - you can optionally name this signal.
		CSignal(const char* name = NO_NAME):CSignalBase(name) {};

	};
};
