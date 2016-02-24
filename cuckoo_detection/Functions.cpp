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

int CheckEnum(char * subkey, int choice)
{
	int flag = 0;
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
												flag = 1;
										}
										else
										{
											if (strstr(res.c_str(), "vmware"))
												flag = 2;
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

int CheckEnum2(char * subkey, int choice)
{
	int flag = 0;
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
										(HKKK, "HardwareID", 0, 0, ValName,
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
												flag = 1;
										}
										else
										{
											if (strstr(res.c_str(), "vmware"))
												flag = 2;
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
				if (strstr(line, "1"))
				{
					createAndWriteFile("CoreNumber.txt");
					printf("Core number is 1. \n");
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

void checkInternet()
{
	try
	{
		bool bConnect = InternetCheckConnection(L"https://www.google.com.tr/", FLAG_ICC_FORCE_CONNECTION, 0);

		int a = GetLastError();
		if (!bConnect)
		{
			createAndWriteFile("Internet.txt");
			printf("No internet. \n");
		}
	}
	catch (...)
	{

	}
}

string getComputerName()
{
	TCHAR nameBuf[MAX_COMPUTERNAME_LENGTH + 2];
	DWORD nameBufSize = sizeof nameBuf - 1;

	GetComputerName(nameBuf, &nameBufSize);
	
	wstring arr_w(nameBuf);
	string arr_s(arr_w.begin(), arr_w.end());
	
	return arr_s;
}

bool dirExists(const string& dirName_in)
{
	DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return false;  //something is wrong with your path!

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return true;   // this is a directory!

	return false;    // this is not a directory!
}

vector<string> split(string str, char delimiter) {
	vector<string> internal;
	stringstream ss(str); // Turn the string into a stream.
	string tok;

	while (getline(ss, tok, delimiter)) {
		internal.push_back(tok);
	}
	return internal;
}

const BYTE* readResource(int id_text)
{
	HRSRC hRes = FindResource(GetModuleHandle(NULL), MAKEINTRESOURCE(id_text), _T("TEXT"));
	DWORD dwSize = SizeofResource(GetModuleHandle(NULL), hRes);
	HGLOBAL hGlob = LoadResource(GetModuleHandle(NULL), hRes);
	const BYTE* pData = reinterpret_cast<const BYTE*>(::LockResource(hGlob));
	return pData;
}

bool replace(std::string& str, const std::string& from, const std::string& to) {
	size_t start_pos = str.find(from);
	if (start_pos == std::string::npos)
		return false;
	str.replace(start_pos, from.length(), to);
	return true;
}

bool command(char * fileName)
{
	string cmdString = "cd c:\\ & dir *";
	string searchCmd = "* /s /p";
	cmdString.append(fileName);
	cmdString.append(searchCmd);
	FILE *fp;
	const char * cmd = cmdString.c_str();
	
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

string eraseString(string str, char val)
{
	str.erase(remove(str.begin(), str.end(), val), str.end());
	return str;
}

void searchFile(int id_text)
{
	WIN32_FIND_DATAA file;
	
	bool flag = false;

	string res(reinterpret_cast< char const* >(readResource(id_text)));
	vector<string> sep = split(res, '\n');
	string targetFile, searchedFile;
	for (int i = 0; i < sep.size(); i++)
	{
		if (!flag)
		{
			replace(sep[i], "\r", "");
			vector<string> sep2 = split(sep[i], '\\');
			for (int j = 0; j < sep2.size() - 1; j++)
			{
				sep2[j].append("\\");
				targetFile.append(sep2[j]);
			}
			targetFile.append("*");
			searchedFile = sep2[sep2.size() - 1];

			HANDLE search_handle = FindFirstFileA(targetFile.c_str(), &file);
			if (search_handle != INVALID_HANDLE_VALUE)
			{
				do
				{
					if (file.cFileName == searchedFile)
					{
						flag = true;
						//CloseHandle(search_handle);
						break;
					}
				} while (FindNextFileA(search_handle, &file));
			}
			//CloseHandle(search_handle);
		}

		else
			break;
	}

	if (flag)
	{
		createAndWriteFile("SandboxFilesFound.txt");
		printf("Sandbox files are founded. \n");
	}
}

void DiskSpace()
{
	//wmic logicaldisk get size,freespace,caption
	bool flag = false;
	char *cmd = "wmic logicaldisk get size";
	shared_ptr<FILE> pipe(_popen(cmd, "r"), _pclose);
	if (pipe)
	{
		char buffer[128];
		string result = "";
		double diskSpace = 0;
		while (!feof(pipe.get())) {
			if (fgets(buffer, 128, pipe.get()) != NULL)
			{
				result = buffer;
				result = eraseString(result, ' ');
				result = eraseString(result, '\r');
				result = eraseString(result, '\n');
				if (result != "Size" && result !="" && result != " ")
				{
					diskSpace = stod(result) / (1024 * 1024 * 1024);
					if (diskSpace < 60)
					{
						createAndWriteFile("diskspace.txt");
						printf("Disk space is smaller than 60Gb. \n");
					}
					break;
				}
			}
		}
	}	
}

void createAndWriteFile(char * fileName)
{
	ofstream myfile;
	myfile.open(fileName);
	myfile << "Hello World.\n";
	myfile.close();
}
