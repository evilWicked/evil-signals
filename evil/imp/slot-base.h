#pragma once

#include <atomic>
#include <functional>
#include <mutex>

#include "signal-base.h"


#define NO_NAME "NO NAME"


namespace evil {

	/**

	See the discussion in [Design Ideas: Signals](@ref designsignals)

	Have drawn upon the basic outline of a signal defined in http://simmesimme.github.io/tutorials/2015/09/20/signal-slot/
	As well as some info on function pointer usage http://www.cplusplus.com/forum/general/159730/
	*/
	class CSignalBase;

	class CSlotBase{

	protected:
		friend class CSignalBase;

		//typedef CSlotBase slot_type;
	//	typedef CSignalBase signal_type;
		//typedef std::function<void(Args...)> callback_type;

		//std::atomic<CSignal<Args...> * > mpcSignal;

		CSignalBase *mpcSignal;
		std::mutex mMutex;

		const std::string mstrName;		//Optional. Used to give the signal a name - helps with debugging

		//callback_type mfuncCallback;

		std::atomic<int> miPriority;
		std::atomic<bool> mbActive;
		std::atomic<bool> mbFireOnce; //default false

									  //atomic functions handle one discrete step change of state of a slot
									  //atomic does NOT refer to atomic in the thread sense.
		void atomicConnect(CSignalBase* x);
		void atomicSetPriority(int x);
		void atomicSetActive(bool x);
		void atomicSetFireOnce(bool x);
		CSignalBase *atomicGetSignal();
		void atomicDisconnect();

		//these handle the actual process used by the slot and may involve interacting with the signal
		//built out of atomic steps
		bool processIsConnected();
		void processConnect(CSignalBase* signal);
		void processConnectOnce(CSignalBase* signal);
		void processDisconnect();
		void processSetActive(bool x);
		void processSetPriority(int x);
		//void processTriggerSlot(Args...);

		//functions only required by the signal class
		CSignalBase *friendGetSignal();
		void friendConnect(CSignalBase* x);
		void friendDisconnect();	
		void friendSetFireOnce(bool x);
		bool friendIsFireOnce();
		//void friendTriggerSlot(Args...);	

	public:
		///Constructor - you can optionally name this slot.
		CSlotBase(const char* name = NO_NAME);

		///Along with normal destruction, this has the additional task of removing itself 
		///from the connected signal to stop it attempting to call the callback in the future.
		~CSlotBase();

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
		std::string name() const;

		///Connect this slot to a signal
		void connect(CSignalBase* signal);

		///Connect this slot to a signal to fire once and be disconnected.
		void connectOnce(CSignalBase* signal) ;

		///Disconnect this slot from the owning signal.
		void disconnect();

		///Are we connected - test we have a signal and the signal is connected to us
		///mainly used for testing
		bool isConnected();

		///Get the owning signal
		CSignalBase* signal();

		///Allow a slot to be turned off - perhaps during a pause state. Stops it receiving a signal
		void setActive(bool x);

		///Get the active state of the slot
		bool active();

		///Change the priority of a slot. zero is normal. Higher fires earlier.
		void setPriority(int x);

		///Get the priority of the slot
		int priority();

		///Check the function is valid
		//bool hasFunction();

	};

};
