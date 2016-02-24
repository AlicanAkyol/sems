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
int CheckEnum(char * subkey, int choice);
int CheckEnum2(char * subkey, int choice);
bool processTools(string process, int size);
bool servicesTools(wchar_t * value);
void checkCoreNumber();
void checkInternet();
void createAndWriteFile(char *fileName);
string getComputerName();
bool dirExists(const string& dirName_in);
vector<string> split(string str, char delimiter);
string eraseString(string str, char val);
void DiskSpace();
bool command(char * fileName);
void searchFile(int id_text);
const BYTE* readResource(int id_text);
bool replace(std::string& str, const std::string& from, const std::string& to);

#endif