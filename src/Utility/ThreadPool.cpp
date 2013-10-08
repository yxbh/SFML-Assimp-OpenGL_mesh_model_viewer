#include "ThreadPool.hpp"

Task::Task(void)
{
} // Task ctor

Task::~Task(void)
{
} // task dtor

// static definitions
ThreadPool::state ThreadPool::s_State = ThreadPool::state::uninitialized;
ThreadPool::task_queue_state ThreadPool::s_TaskQueueState = no_task;

// create thread pool with "threadCount" threads. Merely takes the integer as a suggestion.
ThreadPool::ThreadPool(const int threadCount)
	: m_ThreadPool(threadCount), m_ThreadCount(threadCount), m_TaskQueue()
{
} // ThreadPool ctor

ThreadPool::~ThreadPool(void)
{
	if (this->s_State != uninitialized)
		this->ShutDown();
} // dtor

bool ThreadPool::Init(void)
{
	if (s_State == running)
		return false;

	for (int i = 0; i < m_ThreadCount; ++i)
	{
		m_ThreadPool.emplace_back([this]
		{
			bool running = true;
			while (running)
			{
				if (s_State == shutdown)
					break; // return

				m_TaskQueueMutex.lock();
				if (!m_TaskQueue.empty())
				{
					// carry out task

					m_TaskQueueMutex.unlock();		
				}
				else
					std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}
		}
		);
	}
	return true;
} // ThreadPool::Init()

void ThreadPool::ShutDown(void)
{
	if (this->s_State != uninitialized)
	{
		for (auto &t : m_ThreadPool)
		{
			t.join();
		}
	}
	s_State = uninitialized;
} // end ThreadPool::ShutDown()

void ThreadPool::InitThread(void)
{

}