#include "Func.h"

LONG registerOpenKey(char *value)
{
	HKEY HK = 0;
	return RegOpenKeyExA(HKEY_LOCAL_MACHINE, value, 0, KEY_READ, &HK);
}

HANDLE createFile(char *value)
{
	return CreateFileA(value, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, 0,
		OPEN_EXISTING, 0, 0);
}

HWND findWindowforClass(char *value)
{
	return FindWindowA(value, 0);
}

HWND findWindowforWindow(char *value)
{
	return FindWindowA(0, value);
}

HMODULE loadLibrary(char *value)
{
	return LoadLibraryA(value);
}

DWORD fileExist(char *filename)
{
	return GetFileAttributesA(filename);
}

char * Mac()
{
	PIP_ADAPTER_INFO AdapterInfo;
	DWORD dwBufLen = sizeof(AdapterInfo);
	char *mac_addr = (char*)malloc(17);

	AdapterInfo = (IP_ADAPTER_INFO *)malloc(sizeof(IP_ADAPTER_INFO));
	if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == ERROR_BUFFER_OVERFLOW)
		AdapterInfo = (IP_ADAPTER_INFO *)malloc(dwBufLen);	

	if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == NO_ERROR) {
		PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;
		do {
			sprintf(mac_addr, "%02X:%02X:%02X:%02X:%02X:%02X",
				pAdapterInfo->Address[0], pAdapterInfo->Address[1],
				pAdapterInfo->Address[2], pAdapterInfo->Address[3],
				pAdapterInfo->Address[4], pAdapterInfo->Address[5]);
			pAdapterInfo = pAdapterInfo->Next;
		} while (pAdapterInfo);
	}
	free(AdapterInfo);
	return mac_addr;
}

string resultKey(char * value)
{
	string res;
	for (int ii = 0; ii < strlen((char*)value); ii++)
		res += tolower(value[ii]);
	return res;
}

bool CheckReg(string value, string valueName, string key, string key2)
{
	HKEY HK = 0;
	bool flag = false;
	if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, value.c_str(), 0, KEY_READ, &HK) == ERROR_SUCCESS)
	{
		unsigned long type = 0;
		unsigned long size = 0x100;
		char * reg_value = (char *)LocalAlloc(LMEM_ZEROINIT, size + 10);
		if (ERROR_SUCCESS == RegQueryValueExA(HK, valueName.c_str(), 0, &type, (unsigned char *)reg_value, &size))
		{
			if (key != "")
			{
				if (key2 != "")
				{
					if (type == REG_MULTI_SZ)
					{
						char * video = reg_value;
						while (*(unsigned char *)video)
						{
							string res = resultKey(video);
							if (res.find(key) != std::string::npos || res.find(key2) != std::string::npos)
								flag = true;
							video = &video[strlen(video) + 1];
						}
					}
				}
				else
				{
					string res = resultKey(reg_value);
					if (type == REG_SZ || type == REG_MULTI_SZ)
					{
						if (res.find(key) != std::string::npos)
							flag = true;
					}
				}
			}
			else
				flag = true;
		}
		LocalFree(reg_value);
		RegCloseKey(HK);
	}
	return flag;
}

bool CheckEnum(char * subkey, int choice)
{
	bool flag = false;
	HKEY HK = 0;
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
										&size) == ERROR_SUCCESS)
									{
										string res;
										for (int ii = 0; ii < strlen((char*)ValName); ii++)
										{
											res += tolower(ValName[ii]);
										}
										if (choice == 1)
										{
											if (strstr(res.c_str(), "vbox"))
												flag = true;
										}
										else
										{
											if (strstr(res.c_str(), "vmware"))
												flag = true;
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
	return flag;
}

bool processTools(string process, int size)
{
	bool flag = false;
	HANDLE hProcessSnapshot;
	PROCESSENTRY32 pe32;
	hProcessSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	pe32.dwSize = sizeof (PROCESSENTRY32);
	if (!Process32First(hProcessSnapshot, &pe32)) CloseHandle(hProcessSnapshot);
	do
	{
		string agent = "";
		for (size_t i = 0; i < size; i++)
		{
			agent += pe32.szExeFile[i];
		}
		if (agent == process)
		{
			flag = true;
			break;
		}
	} while (Process32Next(hProcessSnapshot, &pe32));
	CloseHandle(hProcessSnapshot);
	return flag;
}

bool servicesTools(wchar_t* value)
{
	bool flag = false;
	SC_HANDLE hHandle = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT | SC_MANAGER_ENUMERATE_SERVICE | SC_MANAGER_QUERY_LOCK_STATUS | STANDARD_RIGHTS_READ);
	if (NULL == hHandle) {
		printf("OpenSCManager failed (%d)\n", GetLastError()); 
		return -1;
	}
	ENUM_SERVICE_STATUS service;
	DWORD dwBytesNeeded = 0;
	DWORD dwServicesReturned = 0;
	DWORD dwResumedHandle = 0;
	DWORD dwServiceType = SERVICE_WIN32 | SERVICE_DRIVER;
	BOOL retVal = EnumServicesStatus(hHandle, dwServiceType, SERVICE_STATE_ALL,
		&service, sizeof(ENUM_SERVICE_STATUS), &dwBytesNeeded, &dwServicesReturned,
		&dwResumedHandle);
	if (!retVal) {
		if (ERROR_MORE_DATA == GetLastError()) {
			DWORD dwBytes = sizeof(ENUM_SERVICE_STATUS)+dwBytesNeeded;
			ENUM_SERVICE_STATUS* pServices = NULL;
			pServices = new ENUM_SERVICE_STATUS[dwBytes];
			EnumServicesStatus(hHandle, SERVICE_WIN32 | SERVICE_DRIVER, SERVICE_STATE_ALL,
				pServices, dwBytes, &dwBytesNeeded, &dwServicesReturned, &dwResumedHandle);
			for (unsigned iIndex = 0; iIndex < dwServicesReturned; iIndex++) 
			{
				if (wcsstr((pServices + iIndex)-> lpServiceName, value) != NULL)
				{
					flag = true;
					break;
				}
			}
			delete[] pServices;
			pServices = NULL;
		}
		else {
			GetLastError();
		}
	}
	if (!CloseServiceHandle(hHandle)) {
		GetLastError();
	}
	return flag;
}

void checkCoreNumber()
{
	char *cmd = "wmic cpu get NumberOfCores";
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
			count = count + 1;
			if (count == 2)
			{
				if (strstr(line, "4") || strstr(line, "1"))
				{
					printf("Number of core is little %s", line);
				}
			}
		}
		_pclose(fp);
	}
	catch (int e)
	{
		perror("");
	}
}