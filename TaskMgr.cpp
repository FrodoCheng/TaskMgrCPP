#include "TaskMgr.h"

int TASKAPI Initialize(int sz)
{
	return TaskMgr::instance()->init(sz);
}

int TASKAPI UnInitialize()
{
	return TaskMgr::instance()->uninit();
}

int TASKAPI PushTask(Task * task)
{
	return TaskMgr::instance()->push(task);
}

TaskMgr * TaskMgr::instance()
{
	static TaskMgr mgr;
	return &mgr;
}

int TaskMgr::init(int sz)
{
	if (sz < 1)
	{
		return -1;
	}

	if (m_inited.load())
	{
		return -2;
	}
	/**
	 * be careful, must set init flag true ASAP.
	**/
	m_inited.store(true);

	m_exit.store(false);
	std::thread* thrd = nullptr;
	for (int i = 0; i < sz; i++)
	{
		thrd = new (std::nothrow) std::thread(task_cb_fct);
		if (thrd == nullptr)
		{
			return -3;
		}
		m_pool.push_back(thrd);
	}
	return 0;
}

int TaskMgr::uninit()
{
	if (!m_inited.load())
	{
		return -1;
	}
	/**
	 * be careful, must set exit flag true ASAP, so all the work thread can exit ASAP.
	**/
	m_exit.store(true);

	for (auto & thrd : m_pool)
	{
		m_cond.notify_all();
		if (thrd != nullptr && thrd->joinable())
		{
			thrd->join();
			delete thrd;
			thrd = nullptr;
		}
	}

	/**
	 * be careful, must set inited flag false after all the thread handled the task.
	**/
	m_inited.store(false);
	return 0;
}

int TaskMgr::push(Task * p_task)
{
	if (p_task == nullptr)
	{
		return -1;
	}
	// if not inited or if exited, cannot push any task.
	if (!m_inited.load() || m_exit.load())
	{
		return -2;
	}
	std::unique_lock<std::mutex>	lock_guard(m_mtx);
	m_que.push_back(p_task);
	m_cond.notify_one();
	return 0;
}

int TaskMgr::pop(Task ** pp_task)
{
	if (pp_task == nullptr)
	{
		return -1;
	}
	if (!m_inited.load())
	{
		return -2;
	}
	std::unique_lock<std::mutex>	lock_guard(m_mtx);
	if (m_que.empty())
	{
		m_cond.wait(lock_guard);
	}
	if (m_que.empty())
	{
		*pp_task = nullptr;
		return -3;
	}
	*pp_task = m_que.front();
	m_que.pop_front();
	return 0;
}

bool TaskMgr::is_empty()
{
	std::unique_lock<std::mutex>	lock_guard(m_mtx);
	return m_que.empty();
}

void TaskMgr::task_cb_fct()
{
	Task* task = nullptr;
	int ret = 0;
	while (true)
	{
		if (instance()->m_exit.load() && instance()->is_empty())
		{
			break;
		}
		ret = instance()->pop(&task);
		if (ret == 0 && task != nullptr)
		{
			ret = task->Run();
			if (ret != 0)
			{
				//
			}
			delete task;
			task = nullptr;
		}
	}
}
