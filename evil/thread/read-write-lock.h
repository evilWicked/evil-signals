#pragma once

#include "read-write-mutex.h"

namespace evil {

	/**
	The ReadWriteLock enables multiple reader to exist concurrently while restricting writes to to 
	sequential access.  The particular scenario is a list that has readers progressing down it at 
	those times we are not making changes to the structure of the list.  

	The CReadWriteLock has the following behaviours.

		1. Reads can occur in parallel.
		2. Writes can only occur when no other read or write is in progress.
		3. Reads can not occur when a write is in progress.

	*/

	class CReadWriteLock {
		public:
		enum enumType {READ,WRITE};
		private:
		enumType menumType;
		CReadWriteMutex *mpRWmutex;
		public:

			CReadWriteLock(CReadWriteMutex *pRWmutex,enumType type){
				menumType = type;
				mpRWmutex = pRWmutex;

				switch (menumType) {
					case READ: {
						mpRWmutex->readLock();
					}; break;
					case WRITE:{
						mpRWmutex->writeLock();
					}; break;
				}
			};


			~CReadWriteLock(){

				switch (menumType) {
					case READ:{
						mpRWmutex->readUnlock();
					};break;
					case WRITE:{
						mpRWmutex->writeUnlock();
					};break;
				}
			};


	public:
		//Hide and disable/default all constructors and destructors unless specifically overridden
		//this is a mnemonic to force me to think about things. if overridden they are commented
		//CReadWriteLock() = default;
		//~CReadWriteLock() = default;
		CReadWriteLock(const CReadWriteLock& rhs) = delete;
		CReadWriteLock& operator=(const CReadWriteLock& rhs) = delete;
		CReadWriteLock(CReadWriteLock&& other) = delete;
		CReadWriteLock& operator=(CReadWriteLock&& other) = delete;
	};
}