#pragma once

#include <mutex>

#include "slot-base.h"
#include "signal-base.h"
#include "../../thread/read-write-lock.h"


namespace evil {


	//--------------------------------------------------------------------------
	//							Atomic API
	//
	//  Atomic functions are not atomic in the threads sense even though some of them 
	//  manipulate atomic types. They are atomic in the sense that they act on one thing.
	//  Done this way so that we can minimise locking. 
	//
	//--------------------------------------------------------------------------

	

	bool CSignalBase::atomicHasSlot(CSlotBase * slot) {
		CReadWriteLock rwLock(&mRWmutex, CReadWriteLock::READ);

		for (auto it = mlistSlots.begin(); it != mlistSlots.end(); it++) {
			if (*it == slot) {
				return true;
			}
		}
		return false;
	}


	void CSignalBase::atomicAddSlot(CSlotBase *slot) {

		CReadWriteLock rwLock(&mRWmutex, CReadWriteLock::WRITE);

		//gets called once a slot
		if (mlistSlots.empty()) {
			mlistSlots.push_front(slot);
			return;
		}

		//most likely situation
		CSlotBase *b = mlistSlots.back();
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
		//should never get here....
		mlistSlots.push_back(slot);

		return;
	};

	void CSignalBase::atomicRemoveSlot(CSlotBase* slot) {
		CReadWriteLock rwLock(&mRWmutex, CReadWriteLock::WRITE);
		mlistSlots.remove(slot);
	};

	void CSignalBase::atomicClear() {
		CReadWriteLock rwLock(&mRWmutex, CReadWriteLock::WRITE);
		mlistSlots.clear();
	};


	//--------------------------------------------------------------------------
	//							Process API
	//
	// We are bundling different groups of atomic operations into processes. For
	// example check something exists and if not then add it is two atomic operations
	// in which the lock will need to be on the process level.
	//
	//--------------------------------------------------------------------------


	void CSignalBase::processAdd(CSlotBase* slot) {
	
		//is the slot connected to us already?
		if (!atomicHasSlot(slot)) {
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

	void CSignalBase::processAddOnce(CSlotBase* slot) {
		slot->friendSetFireOnce(true);
		processAdd(slot);
	}

	void CSignalBase::processRemove(CSlotBase* slot) {

		//make sure we don't have a reference to the slot
		if (atomicHasSlot(slot)) {
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


	void CSignalBase::processRemoveAll() {


		{	//create some scope
			CReadWriteLock rwLock(&mRWmutex, CReadWriteLock::READ);

			type_slot_list_citer it;
			for (it = mlistSlots.begin(); it != mlistSlots.end(); ++it) {
				(*it)->friendDisconnect();
			}

		}
		
		atomicClear();
	}


	void CSignalBase::processSortSlots() {

		CReadWriteLock rwLock(&mRWmutex, CReadWriteLock::WRITE);

		mlistSlots.sort([](CSlotBase *a, CSlotBase *b) {return a->priority() > b->priority(); });

	}


	/*
		void CSignalBase::processDispatch() {
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
	*/

	//--------------------------------------------------------------------------
	//							Friend API
	// 
	// The friend api is to conceptually separate internal functions called by 
	// slots on signals and vice versa as part of end to end processes that 
	// need to interact with both objects.
	//
	//--------------------------------------------------------------------------

	//functions only required by the slot class
	void CSignalBase::friendAdd(CSlotBase* slot) { 
		processAdd(slot); 
	};

	void CSignalBase::friendAddOnce(CSlotBase* slot) {
		processAddOnce(slot); 
	};

	void CSignalBase::friendRemove(CSlotBase* slot) {
		processRemove(slot); 
	};

	void CSignalBase::friendSortSlots() { 
		processSortSlots();
	};

	bool CSignalBase::friendHasSlot(CSlotBase* slot) {
		return atomicHasSlot(slot); 
	};

	//--------------------------------------------------------------------------
	//							Public API
	//
	//	This is the bit that users see - we want this as small as possible. The less
	//  they can do the less can go wrong
	//
	//--------------------------------------------------------------------------

	CSignalBase::CSignalBase(const char* name /*= NO_NAME */) :mstrName(name) {

	};

	///Destructor. Also handles informing slots that it has been deleted
	CSignalBase::~CSignalBase() {
		processRemoveAll();
	};

	///get the signals name.
	std::string CSignalBase::name()const{
		return mstrName; 
	};


	void CSignalBase::add(CSlotBase *slot) { 
		processAdd(slot); 
	};

	///Add a slot to fire once and be removed.
	void CSignalBase::addOnce(CSlotBase *slot) { 
		processAddOnce(slot);
	};

	///Remove a slot
	void CSignalBase::remove(CSlotBase *slot) { 
		processRemove(slot); 
	};

	///Remove all slots
	void CSignalBase::removeAll() {
		processRemoveAll(); 
	};

	///return how many slots are connected
	int CSignalBase::numSlots() { 
		return (int)mlistSlots.size();
	};
}

//===============================================================================================================