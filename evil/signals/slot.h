#pragma once

#include "imp/signal-base.h"
#include "imp/slot-base.h"


namespace evil {
	
	/**	
	See the discussion in [Design Ideas: Signals](@ref designsignals)
	*/

	template<typename ...Args>
	class CSlot : public CSlotBase {
		
		std::function<void(Args...)> mFunction;

	public:
		CSlot(const char* name = NO_NAME):CSlotBase(name){};

		void attach(std::function<void(Args...)> f) {
			mFunction = f;
		};

		template <typename C>
		void attach_class(C* c, void(C:: *f)(Args...)) {
			mFunction=util::bind_class(c,f);
		};
		

		void triggerSlot(Args... args){
			mFunction(args...);	
		};

		///Check the function is valid
		bool hasFunction() { return !!mFunction; };


	public:
		//Hide and disable/default all constructors and destructors unless specifically overridden
		//this is a mnemonic to force me to think about things. if overridden they are commented
		//CSlot() = default;
		~CSlot() = default;
		CSlot(const CSlot& rhs) = delete;
		CSlot& operator=(const CSlot& rhs) = delete;
		CSlot(CSlot&& other) = delete;
		CSlot& operator=(CSlot&& other) = delete;
	};
};


/*

int connect_member(F&& f, A&& ... a) const {
slots_.insert({++current_id_, std::bind(f, a...)});
return current_id_;
}

// connects a std::function to the signal. The returned
// value can be used to disconnect the function again
int connect(std::function<void(Args...)> const& slot) const {
slots_.insert(std::make_pair(++current_id_, slot));
return current_id_;
}
*/
