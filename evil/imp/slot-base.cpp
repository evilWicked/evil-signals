#pragma once 


#include "slot-base.h"
#include "signal-base.h"

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

	void CSlotBase::atomicConnect(CSignalBase* x) { 
		std::lock_guard<std::mutex> lock(mMutex);
		mpcSignal = x; 
	};

	void CSlotBase::atomicSetPriority(int x) {
		miPriority = x;
	};

	void CSlotBase::atomicSetActive(bool x) {
		mbActive = x; 
	};

	void CSlotBase::atomicSetFireOnce(bool x) {
		mbFireOnce = x; 
	};

	CSignalBase *CSlotBase::atomicGetSignal() {
		return mpcSignal;
	};

	void CSlotBase::atomicDisconnect() { 
		std::lock_guard<std::mutex> lock(mMutex);
		mpcSignal = 0; 
	};

	//--------------------------------------------------------------------------
	//							Process API
	//
	// We are bundling different groups of atomic operations into processes. For
	// example check something exists and if not then add it is two atomic operations
	// in which the lock will need to be on the process level.
	//
	//--------------------------------------------------------------------------


	bool CSlotBase::processIsConnected(){

		if (mpcSignal) {
			return mpcSignal->friendHasSlot(this);
		}
		return false;
	}


	void CSlotBase::processSetPriority(int x) {
		atomicSetPriority(x);
		return mpcSignal->friendSortSlots();
	}


	void CSlotBase::processSetActive(bool x) {
		atomicSetActive(x);
	}


	void CSlotBase::processConnect(CSignalBase* signal) {
		signal->friendAdd(this);
	}


	void CSlotBase::processConnectOnce(CSignalBase* signal) {
		signal->friendAddOnce(this);
	}

	void CSlotBase::processDisconnect() {
		if (mpcSignal) {
			mpcSignal->friendRemove(this);
		}
	};



	//void CSlotBase::processTriggerSlot() {
	//
	//}

	//--------------------------------------------------------------------------
	//							Friend API
	// 
	// The friend api is to conceptually separate internal functions called by 
	// slots on signals and vice versa as part of end to end processes that 
	// need to interact with both objects.
	//
	//--------------------------------------------------------------------------


	CSignalBase *CSlotBase::friendGetSignal() {
		return atomicGetSignal();
	};

	void CSlotBase::friendConnect(CSignalBase* x) {
		atomicConnect(x); 
	};
	
	void CSlotBase::friendDisconnect() {
		atomicDisconnect(); 
	};

	//functions only required by the signal class
	void CSlotBase::friendSetFireOnce(bool x) {
		atomicSetFireOnce(x); 
	};

	bool CSlotBase::friendIsFireOnce() {
		return mbFireOnce; 
	};

	//void friendTriggerSlot(Args...) { 
	//	processTriggerSlot(Args...); 
	//};

	//--------------------------------------------------------------------------
	//							Public API
	//
	//	This is the bit that users see - we want this as small as possible. The less
	//  they can do the less can go wrong
	//
	//--------------------------------------------------------------------------
	
	CSlotBase::CSlotBase(const char* name /*= NO_NAME */):mstrName(name) {
		miPriority = 0;
		mpcSignal = 0;
		mbFireOnce = false;
		//mfuncCallback = 0;
	}

	CSlotBase::~CSlotBase() {
		processDisconnect();
	};

	std::string CSlotBase::name() const {
		return mstrName; 
	};

	void CSlotBase::connect(CSignalBase* signal) {
		processConnect(signal);
	};

	///Connect this slot to a signal to fire once and be disconnected.
	void CSlotBase::connectOnce(CSignalBase* signal) {
		processConnectOnce(signal);
	};


	void CSlotBase::disconnect() {
		processDisconnect();
	};

	///Are we connected - test we have a signal and the signal is connected to us
	///mainly used for testing
	bool CSlotBase::isConnected() {
		return processIsConnected(); 
	};

	///Get the owning signal
	CSignalBase* CSlotBase::signal() {
		return mpcSignal;
	};

	///Allow a slot to be turned off - perhaps during a pause state. Stops it receiving a signal
	void CSlotBase::setActive(bool x) {
		processSetActive(x); 
	};

	///Get the active state of the slot
	bool CSlotBase::active() {
		return mbActive; 
	};

	///Change the priority of a slot. zero is normal. Higher fires earlier.
	void CSlotBase::setPriority(int x) {
		processSetPriority(x); 
	};

	///Get the priority of the slot
	int CSlotBase::priority() {
		return miPriority; 
	};

	///Check the function is valid
	//bool CSlotBase::hasFunction() { 
	//	return !!mfuncCallback; 
	//};



}
	//===============================================================================================================