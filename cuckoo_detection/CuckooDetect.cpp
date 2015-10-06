/*Cuckoo Sandbox detection tool
* v1.0
*/

#include "Func.h"

void agent()
{
	if (processTools("python.", 7))
	{
		createAndWriteFile("agent.txt");
		printf("VirtualBox Detected (agent.py)\n");
	}
		
	if (processTools("pythonw", 7))
	{
		createAndWriteFile("agent.txt");
		printf("VirtualBox Detected (agent.py)\n");
	}
		
}

void pipe()
{
	if (createFile("\\\\.\\pipe\\cuckoo") != INVALID_HANDLE_VALUE)
	{
		createAndWriteFile("pipe.txt");
		printf("VirtualBox Detected (\\\\.\\pipe\\cuckoo)\n");
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

void cuckoo()
{
	if (command())
	{
		createAndWriteFile("cuckoo.txt");
		printf("Cuckoo detected (Files)\n");
	}
		
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
						createAndWriteFile("hook.txt");
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
		printf("Cuckoo detection(Port number 2042)");
	}
	_pclose(p);
}

void cuckooDetect()
{
	checkCoreNumber();
	portScanner();
	agent();
	pipe();
	cuckoo();
	functionHookedByCuckoo();
}

