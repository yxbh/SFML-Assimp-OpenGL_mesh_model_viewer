#pragma once
#include "../common.hpp"

namespace KE
{

	/** \class ThreadSafeQueue
		A thread safe queue modified from the one found at: http://codingways.blogspot.com.au/2012/08/implementing-thread-safe-queue-in-c11.html
		Credits for the original work to the author.
	*/
	template <class T>
	class ThreadSafeQueue
	{
	private :
		std::queue<T> m_Data;
		// check empty() method for description of "mutable"
		mutable std::mutex m_Mut; 
		std::condition_variable m_EmptyCondition;

	public:
		inline void Push(T pValue)
		{
			// block execution here, if other thread already locked mMute!
			std::lock_guard<std::mutex> lock(m_Mut); 
			// if weare here no other thread is owned/locked mMute. so we can modify the internal data
			m_Data.push(pValue); 
			// OK! notify one of threads which was blocked because , queue empty and make them happy!
			m_EmptyCondition.notify_one();
		} // //lock.unlock(); !!!! does not have such method!
		// now mMute is unlocked in destcutor of lock_guard!
 
		inline std::shared_ptr<T> Pop()
		{
			// own the mMute like in Push method excep that lock can be unlocked manually without need to destructor of lock!
			std::unique_lock<std::mutex> lock(m_Mut); 
			// if we are here. mMute is locked and no other thread can access/modify the data!
			// wait() method first checks if mData is not empty, allowes execution to go on. else :
			// unlocks the mMut and waits for signla.
			// because mMute is released other threads have a chance to Push new data into queue 
			// ... in notify this condition variable!
			m_EmptyCondition.wait(lock,[this]{ return !m_Data.empty();});
			// if we are are here, mData is not empty and mMut is locked !
			// be careful ! std::shared_ptr<T> ret(new T(mData.front()) performs 2 memory allocation!
			std::shared_ptr<T> ret = std::shared_ptr<T>(std::make_shared<T>(m_Data.front()));
			m_Data.pop();
			return ret;
		}

		inline void Clear(void)
		{
			std::unique_lock<std::mutex> lock(m_Mut);
			while (!m_Data.empty())
				m_Data.pop();
			/* 
				This method is custom wrote. Therefore it could be incorrect.
				e.g. Not sure if I'm suppose to call notify_one() on m_EmptyCondition below.
			*/
			m_EmptyCondition.notify_one();
		}

		inline std::shared_ptr<T> TryPop(void)
		{
			std::lock_guard<std::mutex> lock(m_Mut); 
			if(m_Data.empty())
				return std::shared_ptr<T>(); // null
			std::shared_ptr<T> ret = std::shared_ptr<T>(std::make_shared<T>(m_Data.front()));
			m_Data.pop();
			return ret;
		}

		// copy of TryPop(void)
		inline bool Poll(T & p_Value)
		{
			std::lock_guard<std::mutex> lock(m_Mut); 
			if(m_Data.empty())
				return false;
			p_Value = m_Data.front();
			m_Data.pop();
			return true;
		}

		inline bool Empty() const
		{
			//  this function is "const", so why are we allowed to modify a member vaiable "mMute"?....
			//we can, because we marked it "mutable"! see definition of mMute.
			std::lock_guard<std::mutex> lock(m_Mut); 
			return m_Data.empty();
		}
	};


	/** \class Queue
		Non-thread-safe wrapper over std::queue.
	*/
	template <class T>
	class Queue
	{
	private :
		std::queue<T> m_Data;

	public:
		inline void Push(T pValue)
		{
			m_Data.push(pValue); 
		}

		inline std::shared_ptr<T> Pop()
		{
			if(m_Data.empty())
				return std::shared_ptr<T>(); // null
			std::shared_ptr<T> ret = std::shared_ptr<T>(std::make_shared<T>(m_Data.front()));
			m_Data.pop();
			return ret;
		}

		inline bool Poll(T & p_Value)
		{
			if(m_Data.empty())
				return false;
			p_Value = m_Data.front();
			m_Data.pop();
			return true;
		}

		inline bool Empty() const
		{
			return m_Data.empty();
		}
	};
} // KE ns
