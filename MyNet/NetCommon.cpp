#include "NetCommon.h"



//
// Auther: parker tang
// Date :   2024-09-16
// Desc:    一些通用工具类函数
//

using namespace Net;

//total 26 tasks
extern char tasks[] = { 'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R', 'S','T','U','V','W','X','Y','Z','I' };

//
//函数： 线程创建
//返回值： 线程句柄
//
HANDLE Net::CreateNetThread(ThreadFunction theFun,void *pData)
{
    DWORD   dwThread;
    
    if (pData == NULL) return  NULL;

    HANDLE  hThread = CreateThread(
                                              NULL,              // default security attributes
                                              0,                    // use default stack size  
                                              theFun,           // thread function name
                                              pData,             // argument to thread function 
                                              0,                    // use default creation flags 
                                             &dwThread);    // returns the thread identifier 

    return hThread;
}

//
//函数： 线程关闭
//参数： 线程句柄
//
void Net::CloseNetThread(HANDLE hThread)
{
    if(hThread) CloseHandle(hThread);
}


//
//函数： 将字符串转换成结构体Transfer_Data
//参数 1： 字符串
//参数 2： 字符串长度
//参数 3:    Transfer_Data变量，存储被转换的结果
//
int Net::String_To_TransferData(char* pData, unsigned int bufferSize, Transfer_Data& tData)
{
    if (!pData || bufferSize < sizeof(Transfer_Data)) return 0;

    strncpy_s(tData.cmdType, pData, sizeof(tData.cmdType));
    strncpy_s(tData.cmd, pData+ sizeof(tData.cmdType), sizeof(tData.cmd));

    char* end;
    tData.id = strtol(pData + sizeof(tData.cmdType) + sizeof(tData.cmd), &end, 10);

    return 1;
}

//
//函数： 将结构体Transfer_Data转换成字符串
//参数 1:    Transfer_Data变量，存储被转换的结果
//参数 2： 字符串
//参数 3： 字符串长度
//
int Net::TransferData_To_String(Transfer_Data& tData, char** ppData, unsigned int bufferSize)
{
    if (!ppData || bufferSize < sizeof(Transfer_Data)) return 0;

    char* pData = *ppData;

    strncpy_s(pData, sizeof(tData.cmdType),tData.cmdType, sizeof(tData.cmdType));
    strncpy_s(pData+ sizeof(tData.cmdType), sizeof(tData.cmd),  tData.cmd, sizeof(tData.cmd));
    sprintf_s(pData + sizeof(tData.cmdType) + sizeof(tData.cmd), sizeof(tData.id), "%d", tData.id);

    return 1;
}

//
//函数： 创建随机数组
//参数 1:    随机数最小值，可为负数
//参数 2： 随机数最大值，可为负数
//返回值： 生成的随机数组，长度为 right - left+1
//
std::vector<int> Net::createRadonNumber(long left, long right)
{
    //c99 使用的API
    /*
          //创建随机数种子
          srand(time(NULL));
          //生成随机数
          rand(); 
    */

    //C++11 使用的API
    std::random_device rd;  // 获取随机数种子
    std::default_random_engine gen(rd());  //默认随机数生成器，其实现和编译器有关
    //std::minstd_rand gen(rd()); //采用线性同余算法的伪随机数生成器
    //std::ranlux24_base gen(rd());//带进位减法的伪随机数生成器
    //std::mt19937 gen(rd()); // 以 rd() 初始化 Mersenne Twister 引擎
    std::uniform_int_distribution<int> distrib(left, right); // 定义分布范围 []
    //std::uniform_real_distribution<double> distrib(1, limit);//定义分布范围 []
    std::vector<int> data;
    long length = right - left + 1;
    for (long i = 0; i < length; i++)
    {
        data.push_back(distrib(gen));
    }

    return data;
}