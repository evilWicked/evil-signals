#pragma once

#include "imp/signal-base.h"
#include "../thread/read-write-lock.h"

namespace evil {

	//we will assume while we are in the evil namespace that other evil things know they are evil and 
	//don't need to be prefixed with evil to access them. So we dont need evil::CPassLock::ALLOW
	//when CPassLock::ALLOW will do.

	/**
	See the discussion in [Design Ideas: Signals](@ref designsignals)
	*/

	template<typename ...Args>
	class CSignal :public CSignalBase {
	
	public:
		///Constructor - you can optionally name this signal.
		CSignal(const char* name = NO_NAME):CSignalBase(name) {};
	
		void dispatch(Args... args) {

			CReadWriteLock rwLock(&mRWmutex, CReadWriteLock::READ);

			for (auto it = mlistSlots.begin(); it != mlistSlots.end(); it++) {
				CSlot<Args...> *slot = static_cast<CSlot<Args...>*>(*it);
				if (slot->active() && slot->hasFunction()) {
					slot->triggerSlot(args...);
				}
			}
		}

	public:
		//Hide and disable/default all constructors and destructors unless specifically overridden
		//this is a mnemonic to force me to think about things. if overridden they are commented
		//CSignal() = default;
		~CSignal() = default;
		CSignal(const CSignal& rhs) = delete;
		CSignal& operator=(const CSignal& rhs) = delete;
		CSignal(CSignal&& other) = delete;
		CSignal& operator=(CSignal&& other) = delete;
	};
};
