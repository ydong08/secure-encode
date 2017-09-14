//--------------------------------------------------------------------//
//                作者：刘文涛                                        //
//                书籍：《网络安全编程技术与实例》                    //
//--------------------------------------------------------------------//
#include <stdio.h>
#include <windows.h> 
#include <time.h>
int Count;
CRITICAL_SECTION cs;
DWORD WINAPI FirstThread(LPVOID param)
{
    while (1)
    {
        EnterCriticalSection(&cs);
        Count++;
        Sleep(1000);
        printf("First(Count:%d)\n", Count);
        LeaveCriticalSection(&cs);
    }
    return 0;
}

DWORD WINAPI SecondThread(LPVOID param)
{
    while (1)
    {
        EnterCriticalSection(&cs);
        Sleep(2000);
        Count++;
        printf("Second(Count:%d)\n", Count);
        LeaveCriticalSection(&cs);
    }
    return 0;
}

int main(int argc, char *argv[])
{
    char Code = 0;
    Count = 0;
    InitializeCriticalSection(&cs);
    printf("Press key 's' to start thread,press Key 't' to stop thread.\n");
    HANDLE FirstHandle = CreateThread(NULL, 0, FirstThread, (void*) &Code, CREATE_SUSPENDED, NULL);
    HANDLE SecondHandle = CreateThread(NULL, 0, SecondThread, (void*) &Code, CREATE_SUSPENDED, NULL);
    while (true)
    {
        scanf("%c", &Code);
        if (Code == 's')
        {
            ResumeThread(FirstHandle);
            ResumeThread(SecondHandle);
        }
        if (Code == 't')
        {
            SuspendThread(FirstHandle);
            SuspendThread(SecondHandle);
        }
    };
    TerminateThread(FirstHandle, 1);
    TerminateThread(SecondHandle, 1);
    return 0;
}
