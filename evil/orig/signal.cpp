#pragma once


#include <mutex>
#include "signal.h"
#include "pass-lock.h"


#include "signal-base.cpp"

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

	template<typename... Args>
	bool CSignal<Args...>::atomicHasSlot(const CSlot *slot)const {

		for (auto it = mlistSlots.begin(); it != mlistSlots.end(); it++) {
			if (*it == slot) {
				return true;
			}
		}
		return false;
	}


	template<typename... Args>
	void CSignal<Args...>::atomicAddSlot(CSlot *slot) {

		//gets called once a slot
		if (mlistSlots.empty()) {
			mlistSlots.push_front(slot);
			return;
		}

		//most likely situation
		CSlot *b = mlistSlots.back();
		if (slot->priority() <= b->priority()) {
			mlistSlots.push_back(slot);
			return;
		}

		//no easy fix so insert it ahead of the first lower priority slot
		for (auto it = mlistSlots.begin(); it != mlistSlots.end(); ++it) {
			if ((*it)->priority() < slot->priority()) {
				mlistSlots.insert(it, slot);
				return;
			}
		}

		//couldn't find a slot of lower priority so add it to the end
		mlistSlots.push_back(slot);

		return;
	};


	//--------------------------------------------------------------------------
	//							Process API
	//
	// We are bundling different groups of atomic operations into processes. For
	// example check something exists and if not then add it is two atomic operations
	// in which the lock will need to be on the process level.
	//
	//--------------------------------------------------------------------------

	template<typename... Args>
	void CSignal<Args...>::processAdd(slot_type *slot) {

		//if we are already changing -> block
		std::lock_guard<std::mutex> changeLock(mChangeMutex);
		//if we are dispatching -> block
		evil::CPassLock dispatchLock(&mDispatchMutex, evil::CPassLock::BLOCK);

		//is the slot connected to us already?
		if (!atomicHaveSlot(slot)) {
			atomicAddSlot(slot);
		}

		//so now the slot is listed by us

		//it should not be connected to anything except nothing or us but lets be overly cautious
		//and assume the most recent call is what the programmer wanted and they perhaps have 
		//not disconnected. Perhaps they have moved the slot to a different signal...

		//is the slot already connected back to us?
		if (slot->friendGetSignal() == this) return;//then done

		//its not plugged into us. is the slot plugged into something else?
		if (slot->friendGetSignal() != 0) {
			slot->friendDisconnect();
		}

		//so now the slot is not connected to anything
		//so connect up to us
		slot->friendConnect(this);

	}

	template<typename... Args>
	void CSignal<Args...>::processAddOnce(CSlot *slot) {
		slot->friendSetFireOnce();
		processAdd(slot);
	}

	template<typename... Args>
	void CSignal<Args...>::processRemove(CSlot *slot) {

		//if we are already changing -> block
		std::lock_guard<std::mutex> changeLock(mChangeMutex);
		//if we are dispatching -> block
		evil::CPassLock dispatchLock(&mDispatchMutex, evil::CPassLock::BLOCK);

		//make sure we don't have a reference to the slot
		if (atomicHaveSlot(slot)) {
			atomicRemoveSlot(slot);
		}
		//so now we know the slot is not listed by us

		if (slot->friendGetSignal() != this) {
			//then either it is null or connected to another signal somehow - someone has connected without
			//disconecting or some such. in any case we dont want to mess with it, so we are done.
			return;
		}

		//this signal is connected to us so disconnect it
		slot->friendDisconnect();
	}

	template<typename... Args>
	void CSignal<Args...>::processRemoveAll() {

		//if we are already changing -> block
		std::lock_guard<std::mutex> changeLock(mChangeMutex);
		//if we are dispatching -> block
		evil::CPassLock dispatchLock(&mDispatchMutex, evil::CPassLock::BLOCK);

		type_slot_list_citer it;
		for (it = mlistSlots.begin(); it != mlistSlots.end(); ++it) {
			(*it)->friendDisconnect();
		}

		atomicClear();
	}

	template<typename... Args>
	void CSignal<Args...>::processSortSlots() {

		//if we are already changing -> block
		std::lock_guard<std::mutex> changeLock(mChangeMutex);
		//if we are dispatching -> block changes
		evil::CPassLock dispatchLock(&mDispatchMutex, evil::CPassLock::ALLOW);

		mlistSlots.sort([](const CSlot *a, CSlot *b) {return a->priority() > b->priority(); });

	}

	template<typename... Args>
	void CSignal<Args...>::processDispatch(Args...) {
		//if we are already changing -> block
		std::lock_guard<std::mutex> changeLock(mChangeMutex);
		//if we are dispatching -> block changes
		evil::CPassLock dispatchLock(&mDispatchMutex, evil::CPassLock::ALLOW);

		for (auto it = mlistSlots.begin(); it != mlistSlots.end(); it++) {
			CSlot *slot = *it;

			if (slot->active() && slot->hasFunction()) {
				slot->friendTriggerSlot(Args..);
			}
		}

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


	//all in the header

}
//===============================================================================================================