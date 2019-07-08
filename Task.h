/**********************************************************************
 * @FileName: Task.h
 * @Author: Frodo Cheng
 * @CreatedTime: May 17th 2019
***********************************************************************/
#pragma once

// For corssplatform
#ifdef WIN32
#define TASKAPI		__stdcall
#else
#define TASKAPI
#endif

class Task
{
public:
	virtual int Run() = 0;
	virtual ~Task() = default;
};

/**
 * @description
 *		Initialize work task pool manager.
 * @param
 *		sz: size of work task threads, default value is 1.
 * @return
 *		if success, return 0; or return non-zero.
 * @note
 *		you must call Initialize and UnInitialize at the same thread, recomand both calls in main thread.
**/
int TASKAPI Initialize(int sz = 1);

/**
 * @description
 *		UnInitialize work task pool manager.
 * @param
 *
 * @return
 *		if success, return 0; or return non-zero.
 * @note
 *		you must call Initialize and UnInitialize at the same thread, recomand both calls in main thread.
**/
int TASKAPI UnInitialize();

/**
 * @description
 *		UnInitialize work task pool manager.
 * @param
 *		task: you must implement class Task interface.
 * @return
 *		if success, return 0; or return non-zero.
 * @note
 *		you must call Initialize successfully and then you can PushTask
 *		if you called UnInitialize and then you cannot PushTask any longer.
**/
int TASKAPI PushTask(Task * task);
