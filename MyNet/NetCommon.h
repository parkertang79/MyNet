#pragma once


#include <tchar.h>
#include <strsafe.h>
#include <vector>
#include <random>
#include <windows.h>

namespace Net
{
#define SERVER_PORT                            8888
#define CLIENT_BASE_PORT                   8890
#define TASK_COUNT                            26
#define TASK_CONCURRENCE_LIMIT     10
#define BUF_SIZE                                   255
#define CLIENT_THREAD_COUNT           1

#define CMD_QUERY             "Query"
#define CMD_OK                   "OK"
#define CMD_CHANGE          "Change"
#define CMD_WAIT                "Wait"
#define CMD_CLOSE              "Close"

enum NetThreadStatus
{
		 INIT,
		 STARTING,
		 STARTED,
		 RUNNING,
		 STOP
};

typedef struct _Transfer_Data
{
	char cmdType[8];
	char cmd[8];
	long int id;
	void clear()
	{
		memset(cmdType, 0, sizeof(cmdType));
		memset(cmd, 0, sizeof(cmd));
		id = 0;
	}
}Transfer_Data;

typedef DWORD WINAPI ThreadFunction(LPVOID lpParam);
HANDLE CreateNetThread(ThreadFunction theFun, void* pData);
void CloseNetThread(HANDLE hThread);


int String_To_TransferData(char* pData, unsigned int bufferSize,Transfer_Data& tData);
int TransferData_To_String(Transfer_Data& tData, char** ppData, unsigned int bufferSize);


std::vector<int> createRadonNumber(long left,long right);

}
