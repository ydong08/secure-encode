#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>
#include <unistd.h>

int main()
{
    char c = 'c';
    // printf("sizeof('c') = %d, sizeof(c) = %d\n", sizeof('c'), sizeof(c));
    // fwide(stdout, 8);
    wchar_t wide_str[] = L"1234567890";
    wchar_t *str = (wchar_t*)malloc((wcslen(wide_str)+1)*sizeof(wchar_t));
    if (str) {
        memcpy(str, wide_str, (wcslen(wide_str)+1)*sizeof(wchar_t));
        wprintf(L"str: %ls\n", str);
        free(str);
    }
    return 0;
}

