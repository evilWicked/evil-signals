#pragma once

#include <mutex>
#include <atomic>
#include <condition_variable>

namespace evil {

	/**
	A Read Write Lock based on Raynal pseudo code from wikipedia. This will allow concurrent reads 
	and sequential writes
	
	https://en.wikipedia.org/wiki/Readers%E2%80%93writer_lock

	*/
	class CReadWriteMutex{

		std::mutex mReadMutex;
		std::mutex mWriteMutex;
	
		unsigned int muNumReads;
		
		public:
			CReadWriteMutex() {
				muNumReads=0;
			};

			void readLock() { 
				mReadMutex.lock(); 
				muNumReads++;
				if (muNumReads == 1) {
					mWriteMutex.lock();
				}
				mReadMutex.unlock();
			}

			void readUnlock() { 
				mReadMutex.lock();
				muNumReads--;
				if (muNumReads == 0) {
					mWriteMutex.unlock();
				}
				mReadMutex.unlock(); 
			}

			void writeLock() { 
				mWriteMutex.lock();
			}

			void writeUnlock() {
				mWriteMutex.unlock(); 
			}

	
	public:
		//Hide and disable/default all constructors and destructors unless specifically overridden
		//this is a mnemonic to force me to think about things. if overridden they are commented
		//CReadWriteMutex() = default;
		~CReadWriteMutex() = default;
		CReadWriteMutex(const CReadWriteMutex& rhs) = delete;
		CReadWriteMutex& operator=(const CReadWriteMutex& rhs) = delete;
		CReadWriteMutex(CReadWriteMutex&& other) = delete;
		CReadWriteMutex& operator=(CReadWriteMutex&& other) = delete;
	};
}