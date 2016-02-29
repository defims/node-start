#include <unistd.h>//system wrapper funtions, macosx,linux and mingw has it.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <stdarg.h>

//http://stackoverflow.com/questions/1023306/finding-current-executables-path-without-proc-self-exe
#ifdef _WIN32//define something for Windows (32-bit and 64-bit, this part is common)
    #include <windows.h>
    char *getExecutablePath()
    {
        char *path = malloc(PATH_MAX);

        if (GetModuleFileName(NULL, path, PATH_MAX) == 0) {
            fprintf(stderr, "can not get executable path.");
            return 0;
        }
        return path;
    }
    int executeCommand(char *cmd)
    {
        char *z;

        STARTUPINFO si = { sizeof(STARTUPINFO) };
        si.cb = sizeof(si);
        //si.dwFlags = STARTF_USESHOWWINDOW;
        si.wShowWindow = SW_HIDE;
        PROCESS_INFORMATION pi;

        for (z = cmd + strlen(cmd); z > cmd; z--)
        {
            if (*z == '/')
                *z = '\\';
        }

        //if (!CreateProcess(NULL, "contents\\node.exe contents\\main.js", NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
        if (!CreateProcess(NULL, cmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi))
        //if (!CreateProcess(NULL, concat("node-start\\node.exe node-start\\main.js ", argv[0]), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi))
        {
            MessageBox(0, "Error On CreateProcess()", "ERROR", MB_OK);
            return 0;
        }
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
        return 1;
    }
    #ifdef _WIN64//define something for Windows (64-bit only)
    #endif
#elif __APPLE__
    #include <mach-o/dyld.h>
    #include "TargetConditionals.h"
    #if TARGET_IPHONE_SIMULATOR// iOS Simulator
    #elif TARGET_OS_IPHONE// iOS device
    #elif TARGET_OS_MAC// Other kinds of Mac OS
        char *getExecutablePath()
        {
            uint32_t size = PATH_MAX;
            char *path = malloc(size);

            if (_NSGetExecutablePath(path, &size) == -1) {
                fprintf(stderr, "can not get executable path.");
                return 0;
            }
            return path;
        }
        int executeCommand(char *cmd)
        {
            return system(cmd);
        }
    #else
    #   error "Unknown Apple platform"
    #endif
#elif __linux__// linux
    char *getExecutablePath()
    {
        char *path = malloc(PATH_MAX);
        ssize_t len;

        if ((len = readlink("/proc/self/exe", path, sizeof(path))) == -1) {
            fprintf(stderr, "can not get executable path.");
            return 0;
        }
        return path;
    }
#elif __unix__ // Unix
#elif defined(_POSIX_VERSION)// POSIX
#else// all unices not caught above
#   error "Unknown compiler"
#endif

char *__dirname;
char *__filename;

char *concat(char *first, ...)
{
    char *str;
    char *result;
    int len;
    va_list vl;

    va_start(vl, first);
    str = first;

    do {
        len += strlen(str);
        str = va_arg(vl, char*);
    } while (str!=NULL);

    result = malloc(len);

    va_start(vl,first);
    str = va_arg(vl, char*);
    strcpy(result, first);

    do {
        //printf("%s\n", str);
        strcat(result, str);
        str = va_arg(vl, char*);
    } while (str != NULL);

    va_end(vl);
    //printf("%s\n", result);

    return result;
}

int set_dirname_filename()
{
    char *path = getExecutablePath();
    char *newcopy = malloc(strlen(path) + 1);
    char *z;

    strcpy(newcopy, path);
    free(path);
    path = NULL;

    for (z = newcopy + strlen(newcopy); z > newcopy; z--)
    {
        if (*z == '/' || *z == '\\')
            break;
    }

    if (z > newcopy)
    {
        __dirname = newcopy;
        *z = '\0';
        __filename = z + 1;
    }
    else
    {
        __filename = newcopy;
        __dirname = NULL;
    }

    return 1;
}
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
int main ( int argc, char **argv )
{
    set_dirname_filename();
    //printf("\n%s\n", __dirname);
    //printf("\n%s\n", __filename);
    //printf("%s\n",concat("ab","cd","ef",NULL));

    //printf("\n%s\n",concat(__dirname,"/contents/node ",__dirname,"/contents/main.js", NULL));
    //system(concat(__dirname,"/contents/node ",__dirname,"/contents/main.js", NULL));
    executeCommand(concat(__dirname,"/contents/node ",__dirname,"/contents/main.js", NULL));
    //windows can execute without .exe

    return 0;
}

