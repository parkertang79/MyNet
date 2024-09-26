#include "ServerMain.h"

#include <winsock2.h>  
#pragma comment(lib,"ws2_32.lib")  
#include <WS2tcpip.h>

//
// Auther: parker tang
// Date :   2024-09-16
// Desc:    服务器类，包括创建线程和线程函数，task管理函数
//


using namespace Net;

extern char tasks[];

ServerMain::ServerMain() :bThreadFlag(Net::INIT), myThread(NULL)
{ 
    memset(taskInLive, 0, sizeof(int) * TASK_COUNT);
    printf("create ServerMain object!\n");

};

//
//函数：  创建服务器线程
//返回值：
//
int ServerMain::Run(void)
{
    if (myThread)
    {
        CloseNetThread(myThread);
    }

    myThread = CreateNetThread(ServerMain::ServerThreadFunction, this);
    return myThread == NULL ? 0 : 1;
}

//
//函数：  服务器线程创建函数
//返回值：
//
DWORD WINAPI ServerMain::ServerThreadFunction(LPVOID lpParam)
{
    ServerMain* pClass =(ServerMain*)  lpParam;
    if (pClass) pClass->ManageTask();
    return 0;
}

//
//函数：  任务管理函数
//返回值：
//
int ServerMain::ManageTask(void)
{
    printf("now start task manager function\n");
    bThreadFlag = Net::RUNNING;

    //create socket
    if (!createSocket())
    {
        bThreadFlag = Net::STOP;
        printf("error when creating socket\n");
        return -1;
    }

    while (true)
    {
        //开始监听  
        if (listen(slisten, 10) == SOCKET_ERROR)
        {
            //printf("listen error !\n");
            Sleep(500);
            continue;
        }

        //receive data
        SOCKET sClient;
        sockaddr_in remoteAddr;
        int nAddrlen = sizeof(remoteAddr);
        char revData[255] = { 0 };

        while (true)
        {
            sClient = accept(slisten, (SOCKADDR*)&remoteAddr, &nAddrlen);
            if (sClient == INVALID_SOCKET)
            {
                printf("accept error !\n");
                break;
            }

            printf("accept  the connection from client !\n");
            int ret = recv(sClient, revData, 255, 0);
            if (ret >= sizeof(Transfer_Data))
            {
  
                revData[ret-1] = 0x00;
                Transfer_Data tData;
                if (String_To_TransferData(revData, sizeof(Transfer_Data), tData))
                {
                    // start task
                    if (0 == strcmp(tData.cmdType, CMD_QUERY))
                    {
                        if (ArrangeTask(tData))
                        {
                            char sendData[255] = { 0 };
                            char* pData = &sendData[0];
                            TransferData_To_String(tData, &pData, 255);
                            send(sClient, sendData, sizeof(sendData), 0);
                        }
                    }
                    else if (0 == strcmp(tData.cmdType, CMD_CLOSE)) //close task
                    {
                        CalcTaskRunTimes(tData);
                        if (ArrangeNewTask(tData))
                        {
                            char sendData[255] = { 0 };
                            char* pData = &sendData[0];
                            TransferData_To_String(tData, &pData, 255);
                            send(sClient, sendData, sizeof(sendData), 0);
                        }
                    }
                }
            }

            closesocket(sClient);
        }

        Sleep(100);
    }

    closeSocket();
    bThreadFlag = Net::STOP;
    printf("now close task manager function\n");
    return 0;
}

//
//函数：  服务器线程状态获取
//返回值：
//
int ServerMain::IsServerStop(void)
{
    return bThreadFlag == Net::STOP ? 1 : 0;
}

//
//函数：   创建socket
//返回值：
//
int ServerMain::createSocket(void)
{
    //初始化WSA  
    WORD sockVersion = MAKEWORD(2, 2);
    WSADATA wsaData;
    if (WSAStartup(sockVersion, &wsaData) != 0)
    {
        return 0;
    }

    //创建套接字  
    slisten = socket(AF_INET, SOCK_STREAM, 0);
    if (slisten == INVALID_SOCKET)
    {
        printf("socket error !");
        WSACleanup();
        return 0;
    }

    //绑定IP和端口  
    sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(SERVER_PORT);
    //sin.sin_addr.S_un.S_addr = INADDR_ANY;
    inet_pton(AF_INET, "127.0.0.1", (void*)&sin.sin_addr);
    if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
    {
        printf("bind error !\n");
        WSACleanup();
        return 0;
    }

    printf("bind port :%d finish!\n", SERVER_PORT);
    return 1;

}

//
//函数：  关闭socket
//返回值：
//
int ServerMain::closeSocket(void)
{
    closesocket(slisten);
    WSACleanup();
    return 1;
}

//
//函数： 安排任务，首先看客户端请求的任务是否可安排，不可则安排新的任务给客户端执行
//返回值：
//
int ServerMain::ArrangeTask(Transfer_Data& tData)
{
    vector<Thread_Task>::iterator it = threadData.begin();
    for (; it != threadData.end(); it++)
    {
        if (it->id == tData.id)
        {
            for (int i = 0; i < TASK_COUNT; i++)
            {
                if (tData.cmd[0] == tasks[i])
                {
                    if ( !it->taskStatus[i]  && taskInLive[i] < TASK_CONCURRENCE_LIMIT - 1)
                    {
                        strncmp(tData.cmdType, CMD_OK, sizeof(tData.cmdType));
                        tData.cmd[0] = tasks[i];
                        tData.cmd[1] = '\0';
                        it->startTimes[i] = GetTickCount();
                        taskInLive[i]++;
                        return 1;
                    }
                    break;
                }
            }
            break;
        } 
    }

    return ArrangeNewTask(tData);;
}

//
//函数： 安排新的任务，不是客户端请求的任务
//返回值：
//
int ServerMain::ArrangeNewTask(Transfer_Data& tData)
{
    vector<Thread_Task>::iterator it = threadData.begin();
    for (; it != threadData.end(); it++)
    {
        if (it->id == tData.id)
        {
            for (int i = 0; i < TASK_COUNT; i++)
            {
                if (!it->taskStatus[i] && taskInLive[i] < TASK_CONCURRENCE_LIMIT -1)
                {
                    strncmp(tData.cmdType, CMD_CHANGE, sizeof(tData.cmdType));
                    tData.cmd[0] = tasks[i];
                    tData.cmd[1] = '\0';
                    it->startTimes[i] = GetTickCount();
                    taskInLive[i]++;
                    return 1;
                }
            }
            strncmp(tData.cmdType, CMD_WAIT, sizeof(tData.cmdType));
            return 1;
        }
    }

    //create new task manage object 
    Thread_Task tTask;
    tTask.id = tData.id;
    tTask.startTimes[0] = GetTickCount();

    for (int i = 0; i < TASK_COUNT; i++)
    {
        if (taskInLive[i] < TASK_CONCURRENCE_LIMIT - 1)
        {
            strncmp(tData.cmdType, CMD_CHANGE, sizeof(tData.cmdType));
            tData.cmd[0] = tasks[i];
            tData.cmd[1] = '\0';
            tTask.startTimes[i] = GetTickCount();
            taskInLive[i]++;
            threadData.push_back(tTask);
            return 1;
        }
    }

    strncmp(tData.cmdType, CMD_WAIT, sizeof(tData.cmdType));
    threadData.push_back(tTask);
    return 1;
}

//
//函数： 计算客户端完成任务的时间，这个时间=客户端执行任务的时间+客户端服务器端通信的时间
//返回值：
//
int ServerMain::CalcTaskRunTimes(Transfer_Data& tData)
{
    vector<Thread_Task>::iterator it = threadData.begin();
    for (; it != threadData.end(); it++)
    {
        if (it->id == tData.id)
        {
            for (int i = 0; i < TASK_COUNT; i++)
            {
                if (tData.cmd[0] == tasks[i])
                {
                    it->endTimes[i] = GetTickCount();
                    if (it->endTimes[i] > it->startTimes[i])
                    {
                        it->runTimes[i] = (it->endTimes[i] - it->startTimes[i]) / 1000; // millisecond --> second
                    }
                    else
                    {
                        it->runTimes[i] = 0;
                    }
                    it->taskStatus[i] = true;
                    taskInLive[i]--;
                    return 1;
                }
            }
            return 0;
        }
    }

    return 0;
}