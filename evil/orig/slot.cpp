#pragma once 

#include "slot.h"

namespace evil {

	//--------------------------------------------------------------------------
	//							Atomic API
	//
	//  Atomic functions are not atomic in the threads sense even though some of them 
	//  manipulate atomic types. They are atomic in the sense that they act on one thing.
	//  Done this way so that we can minimise locking. If we need locks they are placed
	//  at the lowest level possible. Some live at process level. Others at atomic level
	//
	//--------------------------------------------------------------------------



	//--------------------------------------------------------------------------
	//							Process API
	//
	// We are bundling different groups of atomic operations into processes. For
	// example check something exists and if not then add it is two atomic operations
	// in which the lock will need to be on the process level.
	//
	//--------------------------------------------------------------------------

	template<typename... Args>
	bool CSlot<Args...>::processIsConnected() const {

		if (mpcSignal) {
			return mpcSignal->friendHasSlot(this);
		}
		return false;
	}

	template<typename... Args>
	void CSlot<Args...>::processSetPriority(int x) {
		atomicSetPriority(x);
		return mpcSignal->friendSortSlots();
	}


	template<typename... Args>
	void CSlot<Args...>::processTriggerSlot(Args...) {

	}
	//--------------------------------------------------------------------------
	//							Friend API
	// 
	// The friend api is to conceptually separate internal functions called by 
	// slots on signals and vice versa as part of end to end processes that 
	// need to interact with both objects.
	//
	//--------------------------------------------------------------------------

	//all in the header

	//--------------------------------------------------------------------------
	//							Public API
	//
	//	This is the bit that users see - we want this as small as possible. The less
	//  they can do the less can go wrong
	//
	//--------------------------------------------------------------------------




	//===============================================================================================================