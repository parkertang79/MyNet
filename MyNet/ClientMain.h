#pragma once
#include <iostream>
#include <WinSock2.h>
#include <vector>
#include <mutex>
#include "NetCommon.h"

#if _MSC_VER > 1020 // if VC++ version is > 4.2
using namespace std; // std c++ libs implemented in std
#endif

using namespace Net;

//
// 结构体定义一个线程内执行任务的数据，包括任务运行开始和结束时间等
//
typedef struct _Client_Thread_Data
{
	long int id;
	HANDLE  thread;
	bool taskStatus[TASK_COUNT];
	unsigned long startTimes[TASK_COUNT];
	unsigned long endTimes[TASK_COUNT];
	unsigned long runTimes[TASK_COUNT];
	_Client_Thread_Data()
	{
		id = 0;
		thread = NULL;
		memset(taskStatus, 0, sizeof(bool) * TASK_COUNT);
		memset(startTimes, 0, sizeof(unsigned long) * TASK_COUNT);
		memset(endTimes, 0, sizeof(unsigned long) * TASK_COUNT);
		memset(runTimes, 0, sizeof(unsigned long) * TASK_COUNT);
	}
}Client_Thread_Data;


class ClientMain
{
public:
	ClientMain();
	~ClientMain() { printf("now destroy client objects\n"); };

	int Run(void);
	int RunTask(void);
	int IsClientStop(void);
	int TestFun(void);

	static DWORD WINAPI ClientThreadFunction(LPVOID lpParam);


private:
	int createSocket(int id, SOCKET &sclient);
	int closeSocket(SOCKET& sclient);
	void printThreadData(const Client_Thread_Data &threadData);
	void  setThreadFlag(void);

	//the action of task
	int Task_Action(char* cmd,int index);

	int Task_A(int index);
	int Task_B(int index);
	int Task_C(int index);
	int Task_D(int index);
	int Task_E(int index);
	int Task_F(int index);
	int Task_G(int index);
	int Task_H(int index);
	int Task_I(int index);
	int Task_J(int index);
	int Task_K(int index);
	int Task_L(int index);
	int Task_M(int index);
	int Task_N(int index);
	int Task_O(int index);
	int Task_P(int index);
	int Task_Q(int index);
	int Task_R(int index);
	int Task_S(int index);
	int Task_T(int index);
	int Task_U(int index);
	int Task_V(int index);
	int Task_W(int index);
	int Task_X(int index);
	int Task_Y(int index);
	int Task_Z(int index);

	mutex  m_idLock;
	vector<HANDLE> threadPool;
	int threadcount;
	Net::NetThreadStatus  bThreadFlag;
};

