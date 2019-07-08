
#include "Task.h"
#include <iostream>
#include <thread>

template<typename T = int>
class DemoTask : public Task
{
public:
	DemoTask(T i) : i(i) {}
	int Run() override
	{
		char bf[32] = {};
		snprintf(bf, 31, "Run: %d\n", i);
		std::cout << bf;
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		return 0;
	}
	~DemoTask() = default;
private:
	T i;
};

template<typename T = int>
int TaskMgrTest(T = 0)
{
	int ret = 0;
	ret = Initialize(4);

	for (int i = 0; i < 100; i++)
	{
		ret = PushTask(new DemoTask<>(i));
	}

	ret = UnInitialize();

	return 0;
}
