#include <stdio.h>
#include <Windows.h>
#include <string> 
#include <Tlhelp32.h>
#include <iostream>
#include <dirent.h>
#include <thread>
#include <cstdlib>
#include <stdlib.h>
#include <fstream>
#include <sstream>

using namespace std;
#pragma comment(lib, "Mpr")


void driverDetect()
{
	HANDLE hF1 = CreateFileA("\\\\.\\VBoxMiniRdrDN", GENERIC_READ,
	FILE_SHARE_READ | FILE_SHARE_WRITE |
	FILE_SHARE_DELETE, 0,
	OPEN_EXISTING, 0, 0);

	if (hF1 != INVALID_HANDLE_VALUE)
	{
		printf("driverDetect\n");
	}
}

void vboxhookdll()
{
	HMODULE hM1 = LoadLibraryA("VBoxHook.dll");
	if (hM1)
	{
		printf("vboxhookdll\n");
	}
}

void vboxguest()
{
	HKEY HK = 0;
	if ((ERROR_SUCCESS ==
		RegOpenKeyExA(HKEY_LOCAL_MACHINE,
		"SOFTWARE\\Oracle\\VirtualBox Guest Additions", 0,
		KEY_READ, &HK)) && HK)
	{
		printf("vboxguest \n");
	}
}

void vboxtray()
{
	HANDLE hxx = CreateFileA("\\\\.\\pipe\\VBoxTrayIPC ", GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
	if (hxx != INVALID_HANDLE_VALUE)
	{
		printf("vboxtray\n");
	}
}

void vboxtray2()
{
	HWND hY1 = FindWindowA("VBoxTrayToolWndClass", 0);
	HWND hY2 = FindWindowA(0, "VBoxTrayToolWnd");
	if (hY1 || hY2)
	{
		printf("vboxtray2\n");
	}
}

void vboxshared()
{
	unsigned long pnsize = 0x1000;
	char * provider = (char *)LocalAlloc(LMEM_ZEROINIT, pnsize);
	int retv = WNetGetProviderNameA(WNNC_NET_RDR2SAMPLE, provider, &pnsize);
	if (retv == NO_ERROR)
	{
		if (lstrcmpiA(provider, "VirtualBox Shared Folders") == 0)
		{
			printf("vboxshared\n");
		}
	}
}

void vboxsystembios()
{
	HKEY HK = 0;
	if (RegOpenKeyExA
		(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System", 0, KEY_READ,
		&HK) == ERROR_SUCCESS)
	{
		unsigned long type = 0;
		unsigned long size = 0x100;
		char * systembiosversion = (char *)LocalAlloc(LMEM_ZEROINIT, size + 10)
			;
		if (ERROR_SUCCESS ==
			RegQueryValueExA(HK, "SystemBiosVersion", 0, &type,
			(unsigned char *)systembiosversion, &size))
		{
			string res;
			for (int ii = 0; ii < strlen((char*)systembiosversion); ii++)
				res += tolower(systembiosversion[ii]);
			//ToLower((unsigned char *)systembiosversion);
			if (type == REG_SZ || type == REG_MULTI_SZ)
			{		
				if (strstr(res.c_str(), "vbox"))
				{
					printf(res.c_str());
					printf(" - vboxsystembios\n");
				}
			}
			
		}
		LocalFree(systembiosversion);
		type = 0;
		size = 0x200;
		char * videobiosversion = (char *)LocalAlloc(LMEM_ZEROINIT, size + 10);
		if (ERROR_SUCCESS ==
			RegQueryValueExA(HK, "VideoBiosVersion", 0, &type,
			(unsigned char *)videobiosversion, &size))
		{
			if (type == REG_MULTI_SZ)
			{
				char * video = videobiosversion;
				char * result[50];
				while (*(unsigned char *)video)
				{
					string res; int count = 0;
					for (int ii = 0; ii < strlen((char*)video); ii++)
					{
						res += tolower(video[ii]);
					}
					//tolower((unsigned char *)video);
					if (strstr(res.c_str(), "oracle") || strstr(res.c_str(), "virtualbox"))
					{
						printf(res.c_str());
						printf(" - vboxsystembios\n");
					}
					video = &video[strlen(video) + 1];
				}
			}
		}
		LocalFree(videobiosversion);
		RegCloseKey(HK);
	}
}

void vobxenum()
{
	HKEY HK = 0;
	char * subkey = "SYSTEM\\CurrentControlSet\\Enum\\IDE";
	if ((ERROR_SUCCESS ==
		RegOpenKeyExA(HKEY_LOCAL_MACHINE, subkey, 0, KEY_READ, &HK)) && HK)
	{
		unsigned long n_subkeys = 0;
		unsigned long max_subkey_length = 0;
		if (ERROR_SUCCESS ==
			RegQueryInfoKey(HK, 0, 0, 0, &n_subkeys, &max_subkey_length, 0, 0, 0,
			0, 0, 0))
		{
			if (n_subkeys)
			{
				char * pNewKey =
					(char *)LocalAlloc(LMEM_ZEROINIT, max_subkey_length + 1);
				for (unsigned long i = 0; i < n_subkeys; i++)
				{
					memset(pNewKey, 0, max_subkey_length + 1);
					HKEY HKK = 0;
					if (ERROR_SUCCESS ==
						RegEnumKeyA(HK, i, pNewKey, max_subkey_length + 1))
					{
						if ((RegOpenKeyExA(HK, pNewKey, 0, KEY_READ, &HKK) ==
							ERROR_SUCCESS) && HKK)
						{
							unsigned long nn = 0;
							unsigned long maxlen = 0;
							RegQueryInfoKey(HKK, 0, 0, 0, &nn, &maxlen, 0, 0, 0,
								0, 0, 0);
							char * pNewNewKey =
								(char *)LocalAlloc(LMEM_ZEROINIT, maxlen + 1);
							if (RegEnumKeyA(HKK, 0, pNewNewKey, maxlen + 1) ==
								ERROR_SUCCESS)
								{
								HKEY HKKK = 0;
								if (RegOpenKeyExA
									(HKK, pNewNewKey, 0, KEY_READ,
									&HKKK) == ERROR_SUCCESS)
									{
									unsigned long size = 0xFFF;
									unsigned char ValName[0x1000] = { 0 };
									char * result[50];
									if (RegQueryValueExA
										(HKKK, "FriendlyName", 0, 0, ValName,
										& size) == ERROR_SUCCESS)
									{
										string res;
										for (int ii = 0; ii < strlen((char*)ValName); ii++)
										{
											res += tolower(ValName[ii]);
										}
										
										if (strstr(res.c_str(), "vbox"))
										{
											printf(res.c_str());
											printf(" - vobxenum \n");
										}
									}
									RegCloseKey(HKKK);
								}
								}
							LocalFree(pNewNewKey);
							RegCloseKey(HKK);
						}
					}
					
				}
				LocalFree(pNewKey);
			}
		}
		RegCloseKey(HK);	
	}

}

void vobxacpi()
{
	HKEY HK = 0;
	if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\ACPI\\DSDT\\VBOX__", 0, KEY_READ
		, &HK) == ERROR_SUCCESS)
	{
		printf("vboxacpi\n");
	}
}

void agent()
{
	HANDLE hProcessSnapshot;
	PROCESSENTRY32 pe32;
	hProcessSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	pe32.dwSize = sizeof (PROCESSENTRY32);
	if (!Process32First(hProcessSnapshot, &pe32)) CloseHandle(hProcessSnapshot);
	do
	{
		string agent = "";
		for (size_t i = 0; i < 7; i++)
		{
			agent += pe32.szExeFile[i];
		}		
		if (agent == "pythonw" || agent == "python.")
		{
			printf("!! agent found !!\n");
			break;
		}		
	} while (Process32Next(hProcessSnapshot, &pe32));
	CloseHandle(hProcessSnapshot);
}

void pipe()
{
	HANDLE hFind;
	hFind = CreateFileA("\\\\.\\pipe\\cuckoo",
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);
	if (hFind != INVALID_HANDLE_VALUE){
		CloseHandle(hFind);
		printf("/!\\ Pipe \\\\.\\ pipe \\ cuckoo found !\ n");
	}
}

bool command()
{
	char *cmd = "cd c:\\ & dir *cuckoo* /s /p";
	FILE *fp;
	char line[1000];
	string result[100][101];
	int count = 0;
	bool flag = false, flag2 = false, result_flag = false;
	try
	{
		fp = _popen(cmd, "r");
		while (fgets(line, sizeof line, fp) && count<100)
		{
			if (strstr(line, "Directory of"))
			{
				flag = true;
				result[count][0] = line;
			}
			else if (flag && !flag2)
			{
				flag2 = true;
			}
			else if (flag && flag2)
			{
				char *findingFile = strtok(line, " ");
				for (int i = 0; i<3; i++)findingFile = strtok(NULL, " ");
				flag = flag2 = false;
				result[count][count + 1] = findingFile;
				count++;
				result_flag = true;
			}
		}
		_pclose(fp);
	}
	catch (int e)
	{
		perror("");
	}
	return result_flag;
}

void getFiles(char *filename)
{
	bool flag = false;
	try
	{
		DWORD dwattrib;
		dwattrib = GetFileAttributesA(filename);
		if ((dwattrib != INVALID_FILE_ATTRIBUTES) && (dwattrib &
			FILE_ATTRIBUTE_DIRECTORY)){
			printf("%s\n", filename);
		}
	}
	catch (int e)
	{
		perror("");
	}
}

void cuckoo()
{
	if (command())
		printf("Cuckoo file or folder detected");
}

void functionHookedByCuckoo()
{
	string functions[12][100];
	string line;
	ifstream myfile("dllandFunctions.txt");
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			string buf;
			stringstream ss(line);
			string d_f[2];
			while (ss >> buf)
			{
				if (buf.find(",") != std::string::npos)
				{
					buf.erase(buf.end() - 1);
					d_f[0] = buf + ".dll";
				}
				else
					d_f[1] = buf;
			}
			bool flag = false;
			for (size_t i = 0; i < 12; i++)
			{
				if (d_f[0] == functions[i][0])
				{	
					for (size_t j = 0; j < 100; j++)
					{
						if (functions[i][j]=="")
						{
							functions[i][j] = d_f[1];
							break;
						}
					}
					flag = true;
					break;
				}
			}
			if (!flag)
			{
				for (size_t i = 0; i < 12; i++)
				{
					if (functions[i][0] == "")
					{
						functions[i][0] = d_f[0];
						functions[i][1] = d_f[1];
						break;
					}
				}
			}
		}
		myfile.close();
	}
	

	FARPROC addr;
	try
	{		
		for (size_t i = 0; i < 12; i++)
		{
			for (size_t j = 0; j < 100; j++)
			{
				if (functions[i][j] != "")
				{
					addr = GetProcAddress(LoadLibraryA(functions[i][0].c_str()), functions[i][j].c_str());
					if (addr == 0x00000000) continue;
					else if (*(BYTE *)addr == 0xE9) printf("%s is hooked by cuckoo !!\n", functions[i][j].c_str());
				}			
			}
		}
	}
	catch (int e)
	{
		perror("ERROR");
	}	
}

int main()
{
	while (1)
	{
		driverDetect();
		vboxhookdll();
		vboxguest();
		vboxtray();
		vboxtray2();
		vboxshared();
		vobxacpi();
		vobxenum();
		vboxsystembios();
		agent();
		pipe();
		functionHookedByCuckoo();
		cuckoo();		
		Sleep(1000);
	}
	getchar();
}