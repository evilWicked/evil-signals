#pragma once 

#include "evil/slot.h"

namespace evil {

	CSlot::CSlot(const char* name/* = NO_NAME*/) :mstrName(name) {
		miPriority = 0;
		mpcSignal = 0;
	}

	///Along with normal destruction, this has the additional task of removing itself 
	///from the connected signals to stop them attempting to call the callback
	CSlot::~CSlot() {
		disconnect();
	}

	void CSlot::privateConnect(CSignal *signal) {
		mpcSignal = signal;
	}

	void CSlot::privateDisconnect() {
		mpcSignal = 0;
	}

	void CSlot::privateSetPriority(int x) {
		miPriority = x;
	}

	void CSlot::setActive(bool x) {
		mbActive = x; 
	};

	//use the signals handlers to disconnect us
	void CSlot::disconnect() {
		//if we actually are connected
		if (mpcSignal) {
			mpcSignal->remove(this);
		}
	}

	//use the signals handlers to connect us up
	void CSlot::connect(CSignal *signal) {
		signal->add(this);
	}

///allow a slot to be turned off - perhaps during a pause state. stops it receiving a signal

	void CSlot::setPriority(int x) {
		miPriority = x;
		if (mpcSignal) {
			mpcSignal->privateSortSlots();
		}
	}

}