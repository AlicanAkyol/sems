/*Anubis Sandbox detection tool
* v1.0
*/

#include "Func.h"

void sandboxieDetect()
{
	if (GetModuleHandle((LPCWSTR)"sbiedll.dll") != NULL) {
		createAndWriteFile("sandboxie.txt");
		printf("Sandboxie Detected \n");
	}
}

void dbgDetect()
{
	if (GetModuleHandle((LPCWSTR)"dbghlp.dll") != NULL) {
		createAndWriteFile("dbghlp.txt");
		printf("dbghlp Detected \n");
	}
}

void computerNameSandboxes()
{
	string names[10] = { "USER", "ANDY", "COMPUTERNAME", "CUCKOO", "SANDBOX", "NMSDBOX",
		"XXXX-OX", "CWSX", "WILBERT-SC", "XPAMAST-SC"};
	try
	{
		string strName = getComputerName();
		transform(strName.begin(), strName.end(), strName.begin(), ::toupper);
		for (size_t i = 0; i < 10; i++)
		{
			if (strName == names[i].c_str())
			{
				string filename = names[i].append(".txt");
				createAndWriteFile((char *)filename.c_str());
				printf("Sandbox Detected - Computername\n");
			}
		}
	}
	catch (int e){

	}
}

void processNamesDetect()
{
	if (processTools("sample.exe", 10))
	{
		createAndWriteFile("sample_exe.txt");
		printf("Sandbox detected (sample.exe)\n");
	}

	if (processTools("sub.exe", 7))
	{
		createAndWriteFile("sub_exe.txt");
		printf("Sandbox Detected (sub.exe)\n");
	}
}

void someSandboxesDetect()
{
	processNamesDetect();
	try{
		computerNameSandboxes();
	}
	catch (int e){
	}
	try{
		sandboxieDetect();

	}
	catch (int e){
	}
	try{
		dbgDetect();
	}
	catch (int e){
	}
	try{
		checkCoreNumber();
	}
	catch (int e){
	}
	try{
		checkInternet();
	}
	catch (int e){
	}
	try{
		DiskSpace();
	}
	catch (int e){
	}
	try{
		searchFile(IDR_TEXT1);
	}
	catch (int e){
	}
}