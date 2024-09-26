#include "NetCommon.h"



//
// Auther: parker tang
// Date :   2024-09-16
// Desc:    һЩͨ�ù����ຯ��
//

using namespace Net;

//total 26 tasks
extern char tasks[] = { 'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R', 'S','T','U','V','W','X','Y','Z','I' };

//
//������ �̴߳���
//����ֵ�� �߳̾��
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
//������ �̹߳ر�
//������ �߳̾��
//
void Net::CloseNetThread(HANDLE hThread)
{
    if(hThread) CloseHandle(hThread);
}


//
//������ ���ַ���ת���ɽṹ��Transfer_Data
//���� 1�� �ַ���
//���� 2�� �ַ�������
//���� 3:    Transfer_Data�������洢��ת���Ľ��
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
//������ ���ṹ��Transfer_Dataת�����ַ���
//���� 1:    Transfer_Data�������洢��ת���Ľ��
//���� 2�� �ַ���
//���� 3�� �ַ�������
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
//������ �����������
//���� 1:    �������Сֵ����Ϊ����
//���� 2�� ��������ֵ����Ϊ����
//����ֵ�� ���ɵ�������飬����Ϊ right - left+1
//
std::vector<int> Net::createRadonNumber(long left, long right)
{
    //c99 ʹ�õ�API
    /*
          //�������������
          srand(time(NULL));
          //���������
          rand(); 
    */

    //C++11 ʹ�õ�API
    std::random_device rd;  // ��ȡ���������
    std::default_random_engine gen(rd());  //Ĭ�����������������ʵ�ֺͱ������й�
    //std::minstd_rand gen(rd()); //��������ͬ���㷨��α�����������
    //std::ranlux24_base gen(rd());//����λ������α�����������
    //std::mt19937 gen(rd()); // �� rd() ��ʼ�� Mersenne Twister ����
    std::uniform_int_distribution<int> distrib(left, right); // ����ֲ���Χ []
    //std::uniform_real_distribution<double> distrib(1, limit);//����ֲ���Χ []
    std::vector<int> data;
    long length = right - left + 1;
    for (long i = 0; i < length; i++)
    {
        data.push_back(distrib(gen));
    }

    return data;
}