#include "ServerMain.h"

#include <winsock2.h>  
#pragma comment(lib,"ws2_32.lib")  
#include <WS2tcpip.h>

//
// Auther: parker tang
// Date :   2024-09-16
// Desc:    �������࣬���������̺߳��̺߳�����task������
//


using namespace Net;

extern char tasks[];

ServerMain::ServerMain() :bThreadFlag(Net::INIT), myThread(NULL)
{ 
    memset(taskInLive, 0, sizeof(int) * TASK_COUNT);
    printf("create ServerMain object!\n");

};

//
//������  �����������߳�
//����ֵ��
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
//������  �������̴߳�������
//����ֵ��
//
DWORD WINAPI ServerMain::ServerThreadFunction(LPVOID lpParam)
{
    ServerMain* pClass =(ServerMain*)  lpParam;
    if (pClass) pClass->ManageTask();
    return 0;
}

//
//������  ���������
//����ֵ��
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
        //��ʼ����  
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
//������  �������߳�״̬��ȡ
//����ֵ��
//
int ServerMain::IsServerStop(void)
{
    return bThreadFlag == Net::STOP ? 1 : 0;
}

//
//������   ����socket
//����ֵ��
//
int ServerMain::createSocket(void)
{
    //��ʼ��WSA  
    WORD sockVersion = MAKEWORD(2, 2);
    WSADATA wsaData;
    if (WSAStartup(sockVersion, &wsaData) != 0)
    {
        return 0;
    }

    //�����׽���  
    slisten = socket(AF_INET, SOCK_STREAM, 0);
    if (slisten == INVALID_SOCKET)
    {
        printf("socket error !");
        WSACleanup();
        return 0;
    }

    //��IP�Ͷ˿�  
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
//������  �ر�socket
//����ֵ��
//
int ServerMain::closeSocket(void)
{
    closesocket(slisten);
    WSACleanup();
    return 1;
}

//
//������ �����������ȿ��ͻ�������������Ƿ�ɰ��ţ����������µ�������ͻ���ִ��
//����ֵ��
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
//������ �����µ����񣬲��ǿͻ������������
//����ֵ��
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
//������ ����ͻ�����������ʱ�䣬���ʱ��=�ͻ���ִ�������ʱ��+�ͻ��˷�������ͨ�ŵ�ʱ��
//����ֵ��
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