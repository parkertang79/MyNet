// MyNet.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "ClientMain.h"
#include "ServerMain.h"
#include <Windows.h>


//
//函数：打印命令行帮助信息 
//返回值：无意义，一直返回 -1
//
int PrintHelp(void)
{
    printf("This program is used to start a server or client program\n");
    printf("      using MyNet -s to start server program\n");
    printf("      using MyNet -c to start client program\n");
    printf("      using MyNet -s -c to start server and client program\n");
    printf("      using MyNet -t to test function\n");
    return -1;
}

//
//函数：启动服务器端线程
// 参数：
// 1. name :  命令行参数
// 2. server :  服务器类对象
//返回值：服务器线程启动状态
//
int StartServerPrg(char* name, ServerMain  &server)
{
    if (0 == strcmp(name, "-s") || 0 == strcmp(name, "-S"))
    {
        return server.Run();
    }
    return 0;
}

//
//函数：启动客户端线程
// 参数：
// 1. name :  命令行参数
// 2. client :  客户端类对象
//返回值：客户端线程启动状态
//
int StartClientPrg(char* name, ClientMain  &client)
{
    if (0 == strcmp(name, "-c") || 0 == strcmp(name, "-C"))
    {
        return client.Run();
    }
    return 0;
}

//
//函数：测试客户端的函数
// 参数：
// 1. name :  命令行参数
// 2. client :  客户端类对象
//返回值：0 表示不做测试， 1 表示已经测试，不需要运行服务器和客户端线程
//
int StartTestFunPrg(char* name, ClientMain& client)
{
    if (0 == strcmp(name, "-t") || 0 == strcmp(name, "-T"))
    {
        return client.TestFun();
    }
    return 0;
}


//
//函数：等待服务器和客户端线程结束
//参数：
// 1. server :  服务器类对象
// 2. client :  客户端类对象
//返回值：无意义
//
int WaitStopFlag(ServerMain& server, ClientMain& client)
{
    do
    {
        Sleep(5);
        if (server.IsServerStop() && client.IsClientStop()) break;
    } while (true);

    //等待线程完全退出
   Sleep(5);

   return 0;
}

//
//函数：入口函数
//参数：
//1. argc  : 命令行参数个数，如果没有参数，则为 1
//2. argv : 命令行参数的字符串数组
//返回值：-1 表示程序运行有错误 0 表示程序运行正常
//
int main( int argc, char* argv[])
{
    ClientMain  client;
    ServerMain  server;

    if (argc == 2) 
    {
        //启动函数功能性测试
        if (StartTestFunPrg(argv[1], client))
        {
            return 0;
        }

        //只启动服务器线程或者客户端线程
         if(! StartServerPrg(argv[1],server) && !StartClientPrg(argv[1],client) )
         {
             return PrintHelp();
         }
    }
    else if (argc == 3) //同时启动服务器线程和客户端线程
    {
        if (!(StartServerPrg(argv[1],server) && StartClientPrg(argv[2],client)) &&
            !(StartServerPrg(argv[2],server) && StartClientPrg(argv[1],client)))
        {
            return PrintHelp();
        }
    }
    else
    {
        return PrintHelp();
    }

    // 主线程要等待服务器线程和客户端线程结束后才可以退出
    return WaitStopFlag(server,client);
}

