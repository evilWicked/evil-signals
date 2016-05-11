#include "evil/signal.h"

namespace evil {

CSignal::CSignal(const char* name/*= NO_NAME*/) :mstrName(name) {
	mbUseLockGuards = false;
}

CSignal::~CSignal() {
	removeAll();
}

//the private functions are atomic in the sense they only work on one step of the
//process of connecting and disconnecting signals from slots. the public functions 
//handle the chicken and egg problem of slot removing signal that removes slot that
//needs a signal removed....

//normally I would not label them as private. have done so to make it clear in the slot class
//that we are using private friend functionality.

void CSignal::privatePushFrontSlot(CSlot *slot) {

	//inelegant but the gaurd is defined by enclosing scope
	if (mbUseLockGuards) {
		std::lock_guard<std::mutex> lock(mListMutex);
		mlistSlots.push_front(slot);
	}else{
		mlistSlots.push_front(slot);
	}
	
}

void CSignal::privatePushBackSlot(CSlot *slot) {

	//inelegant but the gaurd is defined by enclosing scope
	if (mbUseLockGuards) {
		std::lock_guard<std::mutex> lock(mListMutex);
		mlistSlots.push_back(slot);
	}else{
		mlistSlots.push_back(slot);
	}
}


void CSignal::privateAddSlot(CSlot *slot) {

	//gets called once a slot
	if (mlistSlots.empty()) {
		privatePushFrontSlot(slot);
		return;
	}

	//most likely situation
	type_slot_list_citer it = mlistSlots.end();
	if (slot->priority() <= (*it)->priority()) {
		privatePushBackSlot(slot);
		return;
	}

	//no easy fix so call insert
	privateInsertSlot(slot);

	return;
};


void CSignal::privateRemoveSlot(CSlot *slot){ 

	//inelegant but the gaurd is defined by enclosing scope
	if (mbUseLockGuards) {
		std::lock_guard<std::mutex> lock(mListMutex);
		mlistSlots.remove(slot);
	}else{
		mlistSlots.remove(slot);
	}
}

void CSignal::privateInsertSlot(CSlot *slot) {

	type_slot_list_citer it = mlistSlots.begin();
	while (it != mlistSlots.end()) {
		if ((*it)->priority() < slot->priority()) {
			break;
		}
		it++;
	}
	mlistSlots.insert(it, slot);

	//inelegant but the gaurd is defined by enclosing scope
	if (mbUseLockGuards) {
		std::lock_guard<std::mutex> lock(mListMutex);
		mlistSlots.insert(it, slot);
	}else{
		mlistSlots.insert(it, slot);
	}

}


void CSignal::privateClear() {

	//inelegant but the gaurd is defined by enclosing scope
	if (mbUseLockGuards) {
		std::lock_guard<std::mutex> lock(mListMutex);
		mlistSlots.clear();
	}else{
		mlistSlots.clear();
	}
}


void CSignal::privateSortSlots(){

	//inelegant but the gaurd is defined by enclosing scope
	if (mbUseLockGuards) {
		std::lock_guard<std::mutex> lock(mListMutex);
		mlistSlots.sort([](const CSlot *a, CSlot *b) {return a->priority() > b->priority(); });
	}else{
		mlistSlots.sort([](const CSlot *a, CSlot *b) {return a->priority() > b->priority(); });
	}

}


bool CSignal::privateHaveSlot(CSlot *slot)const{

	type_slot_list_citer it;
	for (it = mlistSlots.begin(); it != mlistSlots.end(); ++it) {
		if (*it == slot) {
			return true;
		}
	}
	return false;
}




void CSignal::add(CSlot *slot) {

	//is the slot connected to us already?
	if (!privateHaveSlot(slot)) {
		privateAddSlot(slot);
	}

	//so now the slot is listed by us

	//it should not be connected to anything except us but lets be overly cautious
	//and assume the most recent call is what the programmer wanted and they perhaps have 
	//not disconnected 

	//Is the slot already connected back to us?
	if (slot->mpcSignal == this) return;//then done

										//is the slot plugged into something else?
	if (slot->mpcSignal != 0) {
		slot->disconnect();
	}

	//so the slot is not connected to anything
	//so connect up to us
	slot->privateConnect(this);

	return; //done
}

void CSignal::remove(CSlot *slot) {

	//make sure we don't have a reference to the slot
	if (privateHaveSlot(slot)) {
		privateRemoveSlot(slot);
	}
	//so now we know the slot is not listed by us

	if (slot->mpcSignal != this) {
		//then either it is null or connected to another signal somehow - someone has connected without
		//disconecting or somesuch. in any case we dont want to mess with it, so we are done.
		return;
	}

	//this signal is connected to us to disconnect it
	slot->privateDisconnect();

}

void CSignal::removeAll() {

	//tell all the slots we have gone
	type_slot_list_citer it;
	for (it = mlistSlots.begin(); it != mlistSlots.end(); ++it) {
		(*it)->privateDisconnect();
	}	

	privateClear();	
}


}