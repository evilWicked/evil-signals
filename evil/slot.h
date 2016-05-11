#pragma once

#include <functional>

#include "config.h"
#include "signal.h"

namespace evil {

	/**
	

	See the discussion in [Design Ideas: Signals](@ref designsignals)

	Have drawn upon the basic outline of a signal defined in http://simmesimme.github.io/tutorials/2015/09/20/signal-slot/
	As well as some info on function pointer usage http://www.cplusplus.com/forum/general/159730/
	*/


	class CSlot {
		friend class CSignal;
		//Optional. Used to give the signal a name - helps with debugging
		const std::string mstrName;
		class CSignal *mpcSignal;
		bool mbUseLockGuards;

		int miPriority;
		bool mbActive;

		void privateConnect(CSignal *signal);
		void privateDisconnect();
		void privateSetPriority(int x);
		void privateSetActive(int x);

	public:

		CSlot(const char* name = NO_NAME);

		///Along with normal destruction, this has the additional task of removing itself 
		///from the connected signals to stop them attempting to call the callback in the future.
		~CSlot();

		///Disconnect this slot from the owning signal.
		void disconnect();

		///Connect this slot to a signal
		void connect(CSignal *signal);

		///Connect this slot to a signal to fire once and be disconnected.
		void connectOnce(CSignal *signal);

		///Allow a slot to be turned off - perhaps during a pause state. Stops it receiving a signal
		bool active() const{ return mbActive; };
		void setActive(bool x);

		///Change the priority of a slot. zero is normal. Higher fires earlier.
		int priority() const{ return miPriority; };
		void setPriority(int x);

		///Turns on the use of lock guards on operations on this slot.  This can be considered experimental and covers
		///the case of a single signal on running on several threads talking to this slot that is on a single thread.
		void useLockGuards(bool x) { mbUseLockGuards = x; }
	};

}