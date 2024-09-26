#include "ClientMain.h"
#include <WS2tcpip.h>

#include "leetcode1.h"


//
// Auther: parker tang
// Date :   2024-09-16
// Desc:    客户端类，包括线程函数和task函数
//


extern char tasks[];

ClientMain::ClientMain():bThreadFlag(Net::INIT)
{
    threadcount = 0;
    printf("create client object \n");
}


//
//函数： 运行功能测试函数
//返回值：需返回 1，表示立即退出程序
//
int ClientMain::TestFun(void)
{
    char cmd[8] = { 0 };
    cmd[0] = 'A';
    //执行 单个任务
    Task_Action(&tasks[0], 0);

    //执行全部的任务
    /*
    for (int i = 0; i < TASK_COUNT; i++)
    {
        Task_Action(&tasks[i], 0);
    }
    */
    return 1;
}

//
//函数： 创建客户端线程池
//返回值：
//
int ClientMain::Run(void)
{
    bThreadFlag = Net::INIT;

    // 先清除以前的线程
    vector<HANDLE>::iterator  it = threadPool.begin();
    for (; it != threadPool.end(); it++)
    {
        if (*it) CloseHandle(*it);
    }

    while (!threadPool.empty()) threadPool.erase(threadPool.begin());

    // 创建线程池
    threadcount = 0;
    for (int i = 0; i < CLIENT_THREAD_COUNT; i++)
    {
        
        HANDLE handle= CreateNetThread(ClientMain::ClientThreadFunction, this);
        threadPool.push_back(handle);
    }

    //设置客户端线程状态，避免主线程退出
    bThreadFlag = Net::RUNNING;

    return 1;
}

//
//函数： 线程创建函数
//返回值：
//
DWORD WINAPI ClientMain::ClientThreadFunction(LPVOID lpParam)
{
    ClientMain* pClass = (ClientMain*)lpParam;
    if (pClass) pClass->RunTask();
    return 0;
}


//
//函数： 线程主运行函数 
//返回值：
//
int ClientMain::RunTask(void)
{
    int id = 0;
    Client_Thread_Data threadData;
    Transfer_Data tData;

    {
        m_idLock.lock();
        threadcount++;
        id = threadcount;
        m_idLock.unlock();
    }

    printf("thread id:%d is running\n", id);
    while (true)
    {
        char cmd[8] = { 0 };
        for (int i = 0; i < TASK_COUNT; i++)
        {
            if (!threadData.taskStatus[i])
            {
                cmd[0] = tasks[i];
                break;
            }
        }

        //finish all tasks,exit thread
        if (cmd[0] < tasks[0]) break;

        //create socket to query task
        SOCKET sclient;
       if (createSocket(id, sclient))
       {
            //query task
            char sendData[255] = { 0 };
            char* pData = &sendData[0];
            tData.clear();
            strncpy_s(tData.cmdType, CMD_QUERY, sizeof(tData.cmdType));
            strncpy_s(tData.cmd, cmd, sizeof(tData.cmd));
            if (TransferData_To_String(tData, &pData, 255))
            {
                send(sclient, sendData, 255, 0);
            }
            
            //get task data from server
            char recData[255] = { 0 };
            tData.clear();
            int ret = recv(sclient, recData, 255, 0);
            if (ret >= sizeof(Transfer_Data))
            {
                String_To_TransferData(recData, 255, tData);
            }
            closeSocket(sclient);

            //server notice wait
            if (0 == strcmp(tData.cmdType, CMD_WAIT))
            {
                Sleep(3000);
                continue;
            }
       }

       if (tData.cmd[0] >= tasks[0])
       {
           int actionID = -1;
           for (int i = 0; i < TASK_COUNT; i++)
           {
               if (tData.cmd[0] == tasks[i])
               {
                   actionID = i;
                   break;
               }
           }

           if (actionID == -1) continue;
           threadData.startTimes[actionID] = GetTickCount();

           Task_Action(tData.cmd, 0);

           threadData.endTimes[actionID] = GetTickCount();
           threadData.runTimes[actionID] = (threadData.endTimes[actionID] - threadData.startTimes[actionID]) / 1000;
           threadData.taskStatus[actionID] = true;


           //notice server to finish task
           if (createSocket(id, sclient))
           {
               char sendData[255] = { 0 };
               char* pData = &sendData[0];
               tData.clear();
               strncpy_s(tData.cmdType, CMD_CLOSE, sizeof(tData.cmdType));
               if (TransferData_To_String(tData, &pData, 255))
               {
                   send(sclient, sendData, 255, 0);
               }

               //get response from server
               char recData[255] = { 0 };
               tData.clear();
               int ret = recv(sclient, recData, 255, 0);
               if (ret >= sizeof(Transfer_Data))
               {
                   String_To_TransferData(recData, 255, tData);
               }
               closeSocket(sclient);

               //server notice wait
               if (0 == strcmp(tData.cmdType, CMD_WAIT))
               {
                   Sleep(3000);
               }
           }
       }

    }   

    printThreadData(threadData);
    setThreadFlag();

    return 1;
}

//
//函数： 打印线程任务完成状态
//返回值：
//
void ClientMain::printThreadData(const Client_Thread_Data& threadData)
{
    int totaltime = 0;
    printf("The thread(id=%03d) task run status:\n", threadData.id);
    for (int i = 0; i < TASK_COUNT; i++)
    {
        totaltime += threadData.runTimes[i];
        printf("       Task %c   run times:%6d  \n", tasks[i], threadData.runTimes[i]);
    }
    printf("       total  run times:%6d  \n\n", totaltime);
}

//
//函数：  设置线程池内线程的状态，只有全部线程要退出，才会设置为 Net::STOP
//返回值：
//
void ClientMain::setThreadFlag(void)
{
    {
        m_idLock.lock();
        threadcount--;
        m_idLock.unlock();
    }

    if(threadcount < 1) bThreadFlag = Net::STOP;
}

//
//函数： 检查线程池线程是否完全退出 
//返回值：
//
int ClientMain::IsClientStop()
{
    return bThreadFlag == Net::STOP ? 1 : 0;
}

//
//函数： 创建socket
//返回值：
//
int ClientMain::createSocket(int id, SOCKET& sclient)
{
    //初始化WSA  
    WORD sockVersion = MAKEWORD(2, 2);
    WSADATA wsaData;
    if (WSAStartup(sockVersion, &wsaData) != 0)
    {
        return 0;
    }

    sclient = socket(AF_INET, SOCK_STREAM, 0);
    if (sclient == INVALID_SOCKET)
    {
        printf("invalid socket!\n");
        WSACleanup();
        return 0;
    }

    sockaddr_in serAddr;
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, "127.0.0.1", (void*)&serAddr.sin_addr);

    if (connect(sclient, (sockaddr*)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
    {  //连接失败 
        printf("connect error,code: %d !\n", WSAGetLastError());
        closesocket(sclient);
        WSACleanup();
        return 0;
    }

    return 1;

}

//
//函数： 关闭socket
//返回值：
//
int ClientMain::closeSocket(SOCKET& sclient)
{
    closesocket(sclient);
    WSACleanup();
    return 1;
}

//
//函数： 执行服务器端安排的任务
// 参数：
// 1.cmd ：  任务名
// 2.index:    任务序列号，与cmd 组合才能决定具体的任务
//返回值：
//
int ClientMain::Task_Action(char* cmd, int index)
{
    if (!cmd) return 0;

    if (cmd[0] == 'A') return Task_A(index);
    if (cmd[0] == 'B') return Task_B(index);
    if (cmd[0] == 'C') return Task_C(index);
    if (cmd[0] == 'D') return Task_D(index);
    if (cmd[0] == 'E') return Task_E(index);
    if (cmd[0] == 'F') return Task_F(index);
    if (cmd[0] == 'G') return Task_G(index);
    if (cmd[0] == 'H') return Task_H(index);
    if (cmd[0] == 'I') return Task_I(index);
    if (cmd[0] == 'J') return Task_J(index);
    if (cmd[0] == 'K') return Task_K(index);
    if (cmd[0] == 'L') return Task_L(index);
    if (cmd[0] == 'M') return Task_M(index);
    if (cmd[0] == 'N') return Task_N(index);
    if (cmd[0] == 'O') return Task_O(index);
    if (cmd[0] == 'P') return Task_P(index);
    if (cmd[0] == 'Q') return Task_Q(index);
    if (cmd[0] == 'R') return Task_R(index);
    if (cmd[0] == 'S') return Task_S(index);
    if (cmd[0] == 'T') return Task_T(index);
    if (cmd[0] == 'U') return Task_U(index);
    if (cmd[0] == 'V') return Task_V(index);
    if (cmd[0] == 'W') return Task_W(index);
    if (cmd[0] == 'X') return Task_X(index);
    if (cmd[0] == 'Y') return Task_T(index);
    if (cmd[0] == 'Z') return Task_Z(index);

    printf("no command :%s",cmd);
    return 0;
}

//
// Run Task A, you can write any function code  here!
// return 1 ,the function run ok
// return 0, the function run fail
int ClientMain::Task_A(int index)
{
    (void)index;

    DWORD oldtimes = GetTickCount();

    //test random function
    /*
    vector<int> data = createRadonNumber(-10, 10);
    for (int i = 0; i < data.size(); i++)
    {
        if (0 == (i % 10)) printf("\n");
        printf("  %3d ", data[i]);
    }
    */

    //在该数组中找出 和为目标值 target  的那 两个 整数
    //twoSum();

    //将 字母异位词 组合在一起
    //groupAnagrams();


    //找出数字连续的最长序列
    //longestConsecutive();

    //将所有 0 移动到数组的末尾
    //moveZeros();

    //返回容器可以储存的最大水量
    //maxArea();

    //返回所有和为 0 且不重复的三元组
    //threeSum();


    //计算按此排列的柱子，下雨之后能接多少雨水
   // trap();

    //给定一个字符串 s ，请你找出其中不含有重复字符的最长子串的长度。
    lengthOfLongestSubstring();

    //calc run times
    DWORD runtimes = (GetTickCount() - oldtimes);
    printf("\nTask_A run times:%d ms\n ", runtimes);

    return 1;
}

//
// Run Task B, you can write any function code  here!
// return 1 ,the function run ok
// return 0, the function run fail
int ClientMain::Task_B(int index)
{
    (void)index;

    return 1;
}

//
// Run Task C, you can write any function code  here!
// return 1 ,the function run ok
// return 0, the function run fail
int ClientMain::Task_C(int index)
{
    (void)index;

    return 1;
}

//
// Run Task D, you can write any function code  here!
// return 1 ,the function run ok
// return 0, the function run fail
int ClientMain::Task_D(int index)
{
    (void)index;

    return 1;
}

//
// Run Task E, you can write any function code  here!
// return 1 ,the function run ok
// return 0, the function run fail
int ClientMain::Task_E(int index)
{
    (void)index;

    return 1;
}

//
// Run Task F, you can write any function code  here!
// return 1 ,the function run ok
// return 0, the function run fail
int ClientMain::Task_F(int index)
{
    (void)index;

    return 1;
}

//
// Run Task G, you can write any function code  here!
// return 1 ,the function run ok
// return 0, the function run fail
int ClientMain::Task_G(int index)
{
    (void)index;

    return 1;
}

//
// Run Task H, you can write any function code  here!
// return 1 ,the function run ok
// return 0, the function run fail
int ClientMain::Task_H(int index)
{
    (void)index;

    return 1;
}

//
// Run Task I, you can write any function code  here!
// return 1 ,the function run ok
// return 0, the function run fail
int ClientMain::Task_I(int index)
{
    (void)index;

    return 1;
}

//
// Run Task J, you can write any function code  here!
// return 1 ,the function run ok
// return 0, the function run fail
int ClientMain::Task_J(int index)
{
    (void)index;

    return 1;
}

//
// Run Task K, you can write any function code  here!
// return 1 ,the function run ok
// return 0, the function run fail
int ClientMain::Task_K(int index)
{
    (void)index;

    return 1;
}

//
// Run Task L, you can write any function code  here!
// return 1 ,the function run ok
// return 0, the function run fail
int ClientMain::Task_L(int index)
{
    (void)index;

    return 1;
}

//
// Run Task M, you can write any function code  here!
// return 1 ,the function run ok
// return 0, the function run fail
int ClientMain::Task_M(int index)
{
    (void)index;

    return 1;
}

//
// Run Task N, you can write any function code  here!
// return 1 ,the function run ok
// return 0, the function run fail
int ClientMain::Task_N(int index)
{
    (void)index;

    return 1;
}

//
// Run Task O, you can write any function code  here!
// return 1 ,the function run ok
// return 0, the function run fail
int ClientMain::Task_O(int index)
{
    (void)index;

    return 1;
}

//
// Run Task P, you can write any function code  here!
// return 1 ,the function run ok
// return 0, the function run fail
int ClientMain::Task_P(int index)
{
    (void)index;

    return 1;
}

//
// Run Task Q, you can write any function code  here!
// return 1 ,the function run ok
// return 0, the function run fail
int ClientMain::Task_Q(int index)
{
    (void)index;

    return 1;
}

//
// Run Task R, you can write any function code  here!
// return 1 ,the function run ok
// return 0, the function run fail
int ClientMain::Task_R(int index)
{
    (void)index;

    return 1;
}

//
// Run Task S, you can write any function code  here!
// return 1 ,the function run ok
// return 0, the function run fail
int ClientMain::Task_S(int index)
{
    (void)index;

    return 1;
}

//
// Run Task T, you can write any function code  here!
// return 1 ,the function run ok
// return 0, the function run fail
int ClientMain::Task_T(int index)
{
    (void)index;

    return 1;
}

//
// Run Task U, you can write any function code  here!
// return 1 ,the function run ok
// return 0, the function run fail
int ClientMain::Task_U(int index)
{
    (void)index;

    return 1;
}

//
// Run Task V, you can write any function code  here!
// return 1 ,the function run ok
// return 0, the function run fail
int ClientMain::Task_V(int index)
{
    (void)index;

    return 1;
}

//
// Run Task W, you can write any function code  here!
// return 1 ,the function run ok
// return 0, the function run fail
int ClientMain::Task_W(int index)
{
    (void)index;

    return 1;
}

//
// Run Task X, you can write any function code  here!
// return 1 ,the function run ok
// return 0, the function run fail
int ClientMain::Task_X(int index)
{
    (void)index;

    return 1;
}

//
// Run Task Y, you can write any function code  here!
// return 1 ,the function run ok
// return 0, the function run fail
int ClientMain::Task_Y(int index)
{
    (void)index;

    return 1;
}

//
// Run Task Z, you can write any function code  here!
// return 1 ,the function run ok
// return 0, the function run fail
int ClientMain::Task_Z(int index)
{
    (void)index;

    return 1;
}
