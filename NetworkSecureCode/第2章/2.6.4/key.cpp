//--------------------------------------------------------------------//
//                作者：刘文涛                                        //
//                书籍：《网络安全编程技术与实例》                    //
//--------------------------------------------------------------------//
#include <windows.h>
#include <stdio.h>
void main()
{
    HKEY hkey;
    char Author[100] = "LiuWenTao";
    char Organization[100] = "Internet";
    char City[100] = "WuHan";
    bool State = true;
    unsigned char KeyBuffer[50];
    DWORD Type = 0;
    DWORD DataLen = 1024;
    memset(KeyBuffer, 0, sizeof(KeyBuffer));
    if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\A New Program\\Information", 0, KEY_ALL_ACCESS, &hkey) != ERROR_SUCCESS)
    {
        if (RegCreateKey(HKEY_CURRENT_USER, "Software\\A New Program\\Information", &hkey) != ERROR_SUCCESS)
        {
            printf("RegCreateKey failed with Erro \n");
            return ;
        }
    }
    if (RegSetValueEx(hkey, "Organization", 0, REG_SZ, (BYTE*)Organization, 100) != ERROR_SUCCESS)
    {
        printf("RegSetValueEx failed with Erro \n");
        return ;
    }
    if (RegSetValueEx(hkey, "Author", 0, REG_SZ, (BYTE*)Author, 100) != ERROR_SUCCESS)
    {
        printf("RegSetValueEx failed with   Erro \n");
        return ;
    }
    if (RegSetValueEx(hkey, "City", 0, REG_SZ, (BYTE*)City, 100) != ERROR_SUCCESS)
    {
        printf("RegSetValueEx failed with Erro \n");
        return ;
    }
    if (RegSetValueEx(hkey, "State", 0, REG_DWORD, (BYTE*) &State, 4) != ERROR_SUCCESS)
    {
        printf("RegSetValueEx failed with  Erro \n");
        return ;
    }
    if (RegCloseKey(hkey) != ERROR_SUCCESS)
    {
        printf("RegCloseKey failed with Erro \n");
        return ;
    }
    if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\A New Program\\Information", 0, KEY_ALL_ACCESS, &hkey) == ERROR_SUCCESS)
    {
        if (RegQueryValueEx(hkey, "Author", 0, &Type, KeyBuffer, &DataLen) == ERROR_SUCCESS)
        {
            printf("Author:%s\n", KeyBuffer);
        }
        else
        {
            printf("RegQueryValueEx failed with Erro \n");
            return ;
        }
        if (RegQueryValueEx(hkey, "Organization", 0, &Type, KeyBuffer, &DataLen) == ERROR_SUCCESS)
        {
            printf("Organization:%s\n", KeyBuffer);
        }
        else
        {
            printf("RegQueryValueEx failed with Erro \n");
            return ;
        }
        if (RegQueryValueEx(hkey, "City", 0, &Type, KeyBuffer, &DataLen) == ERROR_SUCCESS)
        {
            printf("City:%s\n", KeyBuffer);
        }
        else
        {
            printf("RegQueryValueEx failed with Erro \n");
            return ;
        }
        if (RegQueryValueEx(hkey, "State", 0, &Type, KeyBuffer, &DataLen) == ERROR_SUCCESS)
        {
            State = KeyBuffer[0];
            if (State)
            {
                printf("State:true\n");
            }
            else
            {
                printf("State:false\n");
            }
        }
        else
        {
            printf("RegQueryValueEx failed with Erro \n");
            return ;
        }
        if (RegCloseKey(hkey) != ERROR_SUCCESS)
        {
            printf("RegCloseKey failed with Erro \n");
            return ;
        }
    }
    else
    {
        printf("RegOpenKeyEx failed with Erro \n");
        return ;
    }
}
