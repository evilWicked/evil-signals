#pragma once

#include <atomic>
#include <functional>
#include <mutex>

#include "signal.h"


#define NO_NAME "NO NAME"

//template implementation included by the include at the bottom of this file.

namespace evil {

	/**
	
	See the discussion in [Design Ideas: Signals](@ref designsignals)

	Have drawn upon the basic outline of a signal defined in http://simmesimme.github.io/tutorials/2015/09/20/signal-slot/
	As well as some info on function pointer usage http://www.cplusplus.com/forum/general/159730/
	*/
	class CSignal;

template<typename... Args>
	class CSlot {		
		//friend class CSignal<Args...>;

		//typedef CSlot<Args...> slot_type;
		//typedef CSignal<Args...> signal_type;
		typedef std::function<void(Args...)> callback_type;

		//std::atomic<CSignal<Args...> * > mpcSignal;
		CSignal<Args...> *mpcSignal;
		std::mutex mMutex;

		const std::string mstrName;		//Optional. Used to give the signal a name - helps with debugging

		callback_type mfuncCallback;

		std::atomic<int> miPriority;
		std::atomic<bool> mbActive;
		std::atomic<bool> mbFireOnce; //default false

		//atomic functions handle one discrete step change of state of a slot
		//atomic does NOT refer to atomic in the thread sense.
		void atomicConnect(CSignal<Args...> *x) {mpcSignal=x; };
		void atomicSetPriority(int x) { miPriority = x; };
		void atomicSetActive(bool x) { mbActive = x; };
		void atomicSetFireOnce(bool x) { mbFireOnce = x; };
		signal_type *atomicGetSignal() { return mpcSignal; };
		void atomicDisconnect() { mpcSignal=0; };

		//these handle the actual process used by the slot and may involve interacting with the signal
		//built out of atomic steps
		bool processIsConnected()const;
		void processSetActive(bool x) { atomicSetActive(x); };
		void processSetPriority(int x);
		void processTriggerSlot(Args...);
					
		signal_type *friendGetSignal() { return atomicGetSignal(); };
		void friendConnect(slot_type *x) {atomicConnect(x);};
		void friendDisconnect() {atomicDisconnect();};
		void friendTriggerSlot(Args...) { processTriggerSlot(Args...); };
		
		//functions only required by the signal class
		void friendSetFireOnce(bool x) { atomicSetFireOnce(x); };
		bool friendIsFireOnce() const { return mbFireOnce; };

	public:
		///Constructor - you can optionally name this slot.
		CSlot(const char* name = NO_NAME) :mstrName(name) {
			miPriority = 0;
			mpcSignal = 0;
			mbFireOnce = false;
			mfuncCallback = 0;
		}

		///Along with normal destruction, this has the additional task of removing itself 
		///from the connected signal to stop it attempting to call the callback in the future.
		~CSlot();

		/*
		template<typename C,typename F> 
		void attach(C&& c, F&& f){
			mfuncCallback = std::bind(f, c, Args...);
		}

		void attach(C c, F f) {
			mfuncCallback = std::bind(f, c, Args...);
		}
		*/


		///get the slots name.
		std::string name() const { return mstrName; };

		///Connect this slot to a signal
		void connect(signal_type *signal){signal->friendAdd(this);};

		///Connect this slot to a signal to fire once and be disconnected.
		void connectOnce(signal_type *signal){ signal->friendAddOnce(this); };

		///Disconnect this slot from the owning signal.
		void disconnect() { mpcSignal->friendRemove(this); };

		///Are we connected - test we have a signal and the signal is connected to us
		///mainly used for testing
		bool isConnected() const { return processIsConnected(); };

		///Get the owning signal
		signal_type* signal(){return mpcSignal;};

		///Allow a slot to be turned off - perhaps during a pause state. Stops it receiving a signal
		void setActive(bool x) { processSetActive(x); };
		
		///Get the active state of the slot
		bool active() const{ return mbActive; };

		///Change the priority of a slot. zero is normal. Higher fires earlier.
		void setPriority(int x) { processSetPriority(x); };

		///Get the priority of the slot
		int priority() const{ return miPriority; };

		///Check the function is valid
		bool hasFunction() { return !!mfuncCallback; };

		
	};

};

//#include "imp/slot.cpp"