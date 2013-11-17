#ifndef _BH_THREADPOOL_
#define _BH_THREADPOOL_
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <chrono>


class Task
{
private:
	
public:
	Task(void);
	~Task(void);

}; // Task

class ThreadPool
{
	friend class Task;
private:
	static enum state {uninitialized, running, shutdown};
	static state s_State;
	static enum task_queue_state {no_task, has_task};
	static task_queue_state s_TaskQueueState;

	int m_ThreadCount;

	// task queue mutex
	std::mutex m_TaskQueueMutex;
	// thread pool
	std::vector<std::thread> m_ThreadPool;
	// task queue
	std::queue<Task> m_TaskQueue;

public:
	ThreadPool(const int threadCount =3);
	~ThreadPool(void);

	bool Init(void);
	void ShutDown(void);

	// add new task into task queue
	bool NewTask(void (*pFunction)(void));

	static ThreadPool* GetPool(void);

private:
	static void InitThread(void);

}; // ThreadPool



#endif