#pragma once

#include <atomic>
#include <list>
#include <queue>
#include <mutex>

#include "slot.h"

#define NO_NAME "NO NAME"

//template implementation included by the include at the bottom of this file.

namespace evil {

	/**


	See the discussion in [Design Ideas: Signals](@ref designsignals)

	Have drawn upon the basic outline of a signal defined in http://simmesimme.github.io/tutorials/2015/09/20/signal-slot/
	As well as some info on function pointer usage http://www.cplusplus.com/forum/general/159730/

	*/


template<typename... Args>
	class CSignal {
	//	friend class CSlot<Args...>;

		//some typedefs for readability
		//typedef CSlot<Args...> slot_type;
		//typedef CSignal<Args...> signal_type;
		typedef std::list<slot_type *> type_slot_list;
		typedef type_slot_list::iterator type_slot_list_iter;
		typedef type_slot_list::const_iterator type_slot_list_citer;

	/// Optional. Used to give the signal a name - helps with debugging
		const std::string mstrName;
	
		type_slot_list mlistSlots;

		std::mutex mDispatchMutex;//used to control signalling threads
		std::mutex mChangeMutex;//used to control threads that change the list itself

		//atomic functions handle one discrete step in the connection process between slots
		//atomic does NOT refer to atomic in the thread sense even though variables may be an atomic type
		//atomic operations should not themselves create a lock_guard
		void atomicHasSlot(CSlot<Args...>* slot);
		void atomicAddSlot(CSlot<Args...>* slot);
		void atomicRemoveSlot(CSlot<Args...> *slot){mlistSlots.remove(slot);};
		void atomicClear() { mlistSlots.clear();};

		//these handle the actual processes that make changes to the signal. Processes should be complete
		//activities that can be called by either the public or friend api. will They call atomic operations
		//and place lock guards around things as appropriate
		void processAdd(slot_type* slot);
		void processAddOnce(slot_type* slot);
		void processRemove(slot_type* slot);
		void processRemoveAll();
		void processDispatch(Args...);

		//functions only required by the slot class
		void friendAdd(slot_type* slot) { processAdd(slot); };
		void friendAddOnce(slot_type* slot) { processAddOnce(slot); };
		void friendRemove(slot_type* slot) { processRemove(slot); };
		void friendSortSlots() { processSortSlots(); };
		void friendHasSlot(slot_type* slot) { atomicHasSlot(slot); };

	public:
		///Constructor - you can optionally name this signal.
		CSignal(const char* name = NO_NAME):mstrName(name){};

		///Destructor. Also handles informing slots that it has been deleted
		~CSignal() {};

		///get the signals name.
		std::string name()const{ return mstrName; };

		///Add a slot
		void add(slot_type *slot) { processAdd(slot); };

		///Add a slot to fire once and be removed.
		void addOnce(slot_type *slot) { processAddOnce(slot); };

		///Remove a slot
		void remove(slot_type *slot) { processRemove(slot); };

		///Remove all slots
		void removeAll() { processRemove(); };
		
		///return how many slots are connected
		int numSlots() { return (int)mlistSlots.size(); };
	
		///Dispatches the signal to all slots
		void dispatch(Args...) { processDispatch(Args...); };
	};
};

//#include "imp/signal.cpp"