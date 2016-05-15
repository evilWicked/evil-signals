#pragma once


#include <list>
#include <mutex>

#include "slot-base.h"

#define NO_NAME "NO NAME"

//implementation included by the include at the bottom of this file.

namespace evil {

	/**


	See the discussion in [Design Ideas: Signals](@ref designsignals)

	Have drawn upon the basic outline of a signal defined in http://simmesimme.github.io/tutorials/2015/09/20/signal-slot/
	As well as some info on function pointer usage http://www.cplusplus.com/forum/general/159730/

	*/
	class CSlotBase;

	class CSignalBase {
	protected:

		friend class CSlotBase;

	//	typedef CSlotBase slot_type;
	//	typedef CSignalBase signal_type;
		typedef std::list<CSlotBase *> type_slot_list;
		typedef std::list<CSlotBase *> type_slot_list;
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

		bool atomicHasSlot(CSlotBase* slot);
		void atomicAddSlot(CSlotBase* slot);
		void atomicRemoveSlot(CSlotBase* slot);
		void atomicClear();

		//these handle the actual processes that make changes to the signal. Processes should be complete
		//activities that can be called by either the public or friend api. will They call atomic operations
		//and place lock guards around things as appropriate
		void processAdd(CSlotBase* slot);
		void processAddOnce(CSlotBase* slot);
		void processRemove(CSlotBase* slot);
		void processRemoveAll();
		void processSortSlots();
		//void processDispatch(Args...);

		//functions only required by the slot class
		void friendAdd(CSlotBase* slot);
		void friendAddOnce(CSlotBase* slot);
		void friendRemove(CSlotBase* slot);
		void friendSortSlots();
		bool friendHasSlot(CSlotBase* slot);

	public:
		///Constructor - you can optionally name this signal.
		CSignalBase(const char* name = NO_NAME);

		///Destructor. Also handles informing slots that it has been deleted
		~CSignalBase();

		///get the signals name.
		std::string name()const;

		///Add a slot
		void add(CSlotBase *slot);

		///Add a slot to fire once and be removed.
		void addOnce(CSlotBase *slot);

		///Remove a slot
		void remove(CSlotBase *slot);

		///Remove all slots
		void removeAll();

		///return how many slots are connected
		int numSlots();

		///Dispatches the signal to all slots
		//void dispatch(Args...);
	};
};
