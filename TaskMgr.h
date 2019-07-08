#pragma once

#include "Task.h"

#include <thread>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <atomic>

class TaskMgr
{
public:
	static TaskMgr* instance();
	int init(int sz);
	int uninit();
	int push(Task* p_task);
	int pop(Task** pp_task);
	bool is_empty();
private:
	std::atomic<bool>			m_inited	= false;
	std::atomic<bool>			m_exit		= false;
	std::deque<std::thread*>	m_pool		= std::deque<std::thread*>();
	std::deque<Task*>			m_que		= std::deque<Task*>();
	std::mutex					m_mtx;
	std::condition_variable		m_cond;

private:
	static void task_cb_fct();

	TaskMgr() = default;
	~TaskMgr() = default;

	TaskMgr(TaskMgr const&) = delete;
	TaskMgr& operator=(TaskMgr const&) = delete;
};
