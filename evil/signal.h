#pragma once


#include <list>
#include <mutex>

#include "config.h"
#include "slot.h"

namespace evil {

	/**


	See the discussion in [Design Ideas: Signals](@ref designsignals)

	Have drawn upon the basic outline of a signal defined in http://simmesimme.github.io/tutorials/2015/09/20/signal-slot/
	As well as some info on function pointer usage http://www.cplusplus.com/forum/general/159730/

	*/


	class CSignal {

		friend class CSlot;

		//some typedefs for readability
		typedef std::list<CSlot*> type_slot_list;
		typedef type_slot_list::iterator type_slot_list_iter;
		typedef type_slot_list::const_iterator type_slot_list_citer;

		/// Optional. Used to give the signal a name - helps with debugging
		const std::string mstrName;
		type_slot_list mlistSlots;
		bool mbUseLockGuards;

		std::mutex mListMutex;

		void privatePushFrontSlot(CSlot *slot);
		void privatePushBackSlot(CSlot *slot);
		void privateAddSlot(CSlot *slot);
		void privateRemoveSlot(CSlot *slot);
		void privateInsertSlot(CSlot *slot);
		void privateClear();
		void privateSortSlots();
		bool privateHaveSlot(CSlot *slot)const;

	public:
		CSignal(const char* name = NO_NAME);
		///Also handles informing slots that it has gone
		~CSignal();

		///Add a slot to a signal
		void add(CSlot *slot);
		///Add a slot to a signal to fire once and be removed.
		void addOnce(CSlot *slot);
		///Remove a slot from a signal
		void remove(CSlot *slot);
		///Remove all slots
		void removeAll();
		///Dispatches the signal to all slots
		void dispatch();

		///turns on the use of lock guards on all operations of the slot list. Is intended to handle the case
		///of different slots on different threads talking to this signal which is running on a single thread.
		void useLockGuards(bool x) { mbUseLockGuards = x; }
	};
}
