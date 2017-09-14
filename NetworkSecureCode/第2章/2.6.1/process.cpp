//--------------------------------------------------------------------//
//                作者：刘文涛                                        //
//                书籍：《网络安全编程技术与实例》                    //
//--------------------------------------------------------------------//
#include <stdio.h>
#include <windows.h>
void main()
{
    char WindowsDirectory[MAX_PATH];
    char CommandLine[MAX_PATH];
    DWORD ExitCode;
    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
    STARTUPINFO si;
    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);
    si.wShowWindow = SW_SHOW;
    si.dwFlags = STARTF_USESHOWWINDOW;
    GetWindowsDirectory(WindowsDirectory, MAX_PATH);
    strcpy(CommandLine, WindowsDirectory);
    strcat(CommandLine, "\\NotePad.exe");
    BOOL ret = CreateProcess(NULL, CommandLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
    if (ret)
    {
        CloseHandle(pi.hThread);
        WaitForSingleObject(pi.hProcess, INFINITE);
        GetExitCodeProcess(pi.hProcess, &ExitCode);
        printf("%d\n", ExitCode);
        CloseHandle(pi.hProcess);
    }
}
