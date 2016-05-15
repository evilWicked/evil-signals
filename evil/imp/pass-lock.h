


/**A variation on a lock gaurd to cover overlapping threads in which we want the lock to be created the first time
a thread holding the lock hits it and to be released when the least instance of the thread leaves it. 
Successive threads that hit the lock while it is active are allowed to proceed. It is used to allow some threads 
to proceed and not others.

If contructed with CPassLock::ALLOW it will continue if another thread holds the lock, 
if CPassLock::BLOCK is used it it blocks. Think of it as a doorman. 

It is easier to visual wise with brackets. Square brackets are locks curly ones threads denote threads.
Normally you have   {a[  ]a}  {b [  ]  b} ... what we want is    {a [ {b  a}   ] b} 
ie the mutext to be locked on entering a and released on exiting b while some other threads are always blocked

The particular scenario is a list of callbacks in which we want the list to be locked for changes but 
not for dispatches. We want parallel dispatching of events to occur while the list blocks changes 
but we want the list to be unlocked for changes when no dispatches are actively walking the list.

To do this we will use reference counting to create the lock when the constructor is called from zero and 
release the lock when the destructor returns to zero.

Exception safety is ensured by using the same trick as a lock_guard to ensure it is destroyed when 
it leaves scope

Detail:
-------

CPassLock::BLOCK. This causes the lock to behave exactly the same as a lock guard. If it can obtain the mutex
it will lock it and proceed otherwise it will block until the mutex is released

CPassLock::ALLOW. This will allways process past the lock. It will try the lock and increment the reference
count - this will block any other threads using lock_guards or CPassLock::BLOCK On destruction it will check
the reference count and will release the lock once the count hits zero.

Like I said; it's a doorman. A listers can always get past the doorman, everyone else has to wait until it is not 
busy.

Usage Scenario:
---------------
The usage scenario I intended it for is on a list where several threads can be reading from the list but I dont want
changes to the structure of the list to occur while there are active iterators on the list. 

To stop the read operations occuring while changes are occuring I use a normal lock guard as well.

std::mutex change_mutex;
std::mutex read_mutex;

### Change Thread

{
	std::lock_guard<std::mutex> change_lock(change_mutex);  //  <-- protects all threads against changes
	evil::CPassLock(&read_mutex,evil::CPassLock:BLOCK); // <--will only get past this if no reads are occuring

}



### Read Thread

{
	std::lock_guard<std::mutex> change_lock;		    // <-- Will only get past this if no changes are occuring
	evil::CPassLock(&read_mutex,evil::CPassLock:ALLOW); // <--Will always get past this and will stop BLOCK'ed threads
											  

}


*/

#include <mutex>
#include <unordered_map>

namespace evil {

	class CPassLock {
		typedef std::mutex* const mutex_ptr;
	public:
		enum access_type {ALLOW,BLOCK};	
		typedef std::unordered_map<std::mutex*, unsigned int> mutex_ref_table;

	private:
		static mutex_ref_table mmapMutexRef;
		const access_type menumType;
		mutex_ptr mMutex;
	public:

		CPassLock(mutex_ptr mtx, const access_type access):mMutex(mtx),
														   menumType(access)
		{
			mmapMutexRef[mtx]++;

			switch (menumType) {
				case ALLOW: {
					mMutex->try_lock();
				}; break;
				case BLOCK: {
					mMutex->lock();
				}; break;
			}
		}


		~CPassLock() {

			 mmapMutexRef[mMutex]--;
			if (!mmapMutexRef[mMutex]) {
				mMutex->unlock();
				//mmapMutexRef.erase(mMutex); - do we need a cleanup - probably not
			}
		};
	};

CPassLock::mutex_ref_table CPassLock::mmapMutexRef;

}


