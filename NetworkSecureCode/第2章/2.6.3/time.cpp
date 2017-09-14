//--------------------------------------------------------------------//
//                作者：刘文涛                                        //
//                书籍：《网络安全编程技术与实例》                    //
//--------------------------------------------------------------------//
#define _WIN32_WINNT 0x0500
#include <stdio.h>
#include <conio.h>
#include <windows.h>
void main()
{
    HANDLE Timer = NULL;
    LARGE_INTEGER liDueTime;
    liDueTime.QuadPart =  - 10000000;
    int KeyInfo;
    while (1)
    {
        Timer = CreateWaitableTimer(NULL, TRUE, "NewTimer");
        if (Timer == NULL)
        {
            printf("CreateWaitableTimer with error %d\n", GetLastError());
            return ;
        }
        if (!SetWaitableTimer(Timer, &liDueTime, 0, NULL, NULL, 0))
        {
            printf("SetWaitableTimer with error %d\n", GetLastError());
            CloseHandle(Timer);
            return ;
        }
        if (WaitForSingleObject(Timer, INFINITE) != WAIT_OBJECT_0)
        {
            printf("WaitForSingleObject with error %d\n", GetLastError());
            CloseHandle(Timer);
            return ;
        }
        else
        {
            static int number = 0;
            number++;
            printf("%d\n", number);
        }
        CloseHandle(Timer);
    }
    return ;
}
