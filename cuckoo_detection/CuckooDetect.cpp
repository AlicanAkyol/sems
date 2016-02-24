/*Cuckoo Sandbox detection tool
* v1.0
*/

#include "Func.h"

int GetModules(DWORD processID)
{
	HMODULE hMods[1024];
	HANDLE hProcess;
	DWORD cbNeeded;
	unsigned int i;

	processID = GetCurrentProcessId();
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, processID);
	if (NULL == hProcess)
		return 1;

	if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
	{
		for (i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
		{
			TCHAR szModName[MAX_PATH];
			wstring arr_w(szModName);
			string arr_s(arr_w.begin(), arr_w.end());
			
			if (GetModuleFileNameEx(hProcess, hMods[i], szModName,
				sizeof(szModName) / sizeof(TCHAR)))
			{
				//_tprintf(TEXT("\\t%s (0x%08X)\\n"), szModName, hMods[i]);
				transform(arr_s.begin(), arr_s.end(), arr_s.begin(), ::toupper);
				if (arr_s.find("CUCKOOMON.DLL") != std::string::npos) {
					createAndWriteFile("cuckoomondll.txt");
					printf("Cuckoo detected (cuckoomon.dll)\n");
				}
			}
		}
	}

	CloseHandle(hProcess);

	return 0;
}

void getDLLList()
{
	DWORD aProcesses[1024];
	DWORD cbNeeded;
	DWORD cProcesses;
	unsigned int i;

	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
		exit;

	cProcesses = cbNeeded / sizeof(DWORD);

	for (i = 0; i < 1; i++)
	{
		GetModules(aProcesses[i]);
	}
}

void agent()
{
	if (processTools("python.", 7))
	{
		createAndWriteFile("agent.txt");
		printf("Cuckoo Detected (agent.py)\n");
	}
		
	if (processTools("pythonw", 7))
	{
		createAndWriteFile("agent.txt");
		printf("Cuckoo Detected (agent.py)\n");
	}
		
}

void pipe()
{
	if (createFile("\\\\.\\pipe\\cuckoo") != INVALID_HANDLE_VALUE)
	{
		createAndWriteFile("pipe.txt");
		printf("Cuckoo Detected (\\\\.\\pipe\\cuckoo)\n");
	}
}

void cuckoo()
{
	if (command("cuckoo"))
	{
		createAndWriteFile("cuckoo.txt");
		printf("Cuckoo detected (Files)\n");
	}
		
}

void functionHookedByCuckoo()
{
	string functions[12][100];
	string res(reinterpret_cast< char const* >(readResource(IDR_TEXT3)));
	vector<string> sep = split(res, '\n');

	for (int i = 0; i < sep.size(); i++)
	{
		replace(sep[i], "\r", "");
		string buf;
		stringstream ss(sep[i]);
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
					if (functions[i][j] == "")
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

	/*

	ifstream myfile("dllandFunctions.txt");
	string line;

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
						if (functions[i][j] == "")
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
	
	*/


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
					else if (*(BYTE *)addr == 0xE9) {
						createAndWriteFile("functionHookedByCuckoo.txt");
						printf("Cuckoo detected (Function hook)!!\n"); break;
					}
				}
			}
		}
	}
	catch (int e)
	{
		perror("ERROR");
	}
}

void portScanner()
{
	char buf[10000];
	FILE *p = _popen("netstat -an", "r");
	string s;
	for (size_t count; (count = fread(buf, 1, sizeof(buf), p));)
	{
		s += string(buf, buf + count);
	}
	if (s.find(":2042") != std::string::npos)
	{
		createAndWriteFile("portnumber.txt");
		printf("Cuckoo Sandbox is detected. Port number 2042. \n");
	}
	_pclose(p);
}

void filesAndFolderCheck()
{

}

void cuckoomonDetect()
{
	if (GetModuleHandle((LPCWSTR)"cuckoomon.dll") != NULL) {
		createAndWriteFile("cuckoomon.txt");
		printf("Cuckoo Detected \n");
	}
}

void cuckooDetect()
{
	try{
		getDLLList();
	}
	catch (int e)
	{

	}
	
	try{
		agent();
	}
	catch (int e)
	{

	}
	try{
		portScanner();
	}
	catch (int e)
	{

	}
	try{
		filesAndFolderCheck();
	}
	catch (int e)
	{

	}
	try{
		cuckoomonDetect();
	}
	catch (int e)
	{

	}
	try{
		cuckoo();
	}
	catch (int e)
	{

	}
	try{
		pipe();
	}
	catch (int e)
	{

	}
	try{
		functionHookedByCuckoo();
	}
	catch (int e)
	{

	}
}

