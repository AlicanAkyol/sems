#ifndef Functions 
#define Functions

#include "VirtualandCuckooDetect.h"

LONG registerOpenKey(char *value);
HWND findWindowforClass(char *value);
HWND findWindowforWindow(char *value);
HANDLE createFile(char *value);
HMODULE loadLibrary(char *value);
DWORD fileExist(char *filename);
string resultKey(char * value);
char * Mac();
bool CheckReg(string value, string valueName, string key, string key2);
bool CheckEnum(char * subkey, int choice);
bool processTools(string process, int size);
bool servicesTools(wchar_t * value);
void checkCoreNumber();
void createAndWriteFile(char *fileName);
TCHAR * getComputerName();
#endif