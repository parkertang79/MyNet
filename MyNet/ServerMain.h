#pragma once

#include <iostream>
#include <WinSock2.h>
#include <vector>
#include "NetCommon.h"

#if _MSC_VER > 1020 // if VC++ version is > 4.2
using namespace std; // std c++ libs implemented in std
#endif

using namespace Net;


typedef struct _Thread_Task
{
	long int id;
	bool taskStatus[TASK_COUNT];
	unsigned long startTimes[TASK_COUNT];
	unsigned long endTimes[TASK_COUNT];
	unsigned long runTimes[TASK_COUNT];
	_Thread_Task()
	{
		id = 0;
		memset(taskStatus, 0, sizeof(bool) * TASK_COUNT);
		memset(startTimes, 0, sizeof(unsigned long) * TASK_COUNT);
		memset(endTimes, 0, sizeof(unsigned long) * TASK_COUNT);
		memset(runTimes, 0, sizeof(unsigned long) * TASK_COUNT);
	}
}Thread_Task;

typedef vector<Thread_Task, allocator<Thread_Task> > TVECTOR;

class ServerMain
{
public:
	ServerMain();
	~ServerMain() { printf("destroy ServerMain object!\n"); };

	int Run(void);
	int ManageTask(void);
	int IsServerStop(void);

	static DWORD WINAPI ServerThreadFunction(LPVOID lpParam);

private:
	int createSocket(void);
	int closeSocket(void);

	int ArrangeTask(Transfer_Data& tData);
	int ArrangeNewTask(Transfer_Data& tData);
	int CalcTaskRunTimes(Transfer_Data& tData);

	Net::NetThreadStatus  bThreadFlag;
	vector<Thread_Task> threadData;
	int taskInLive[TASK_COUNT];
	HANDLE  myThread;
	SOCKET slisten;

};

