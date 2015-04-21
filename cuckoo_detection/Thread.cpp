#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <dirent.h>
#include <stdio.h>
#include <string> 
#include <Tlhelp32.h>
#include <iostream>
#include <dirent.h>
#include <thread>
#include "header1.h"

#define MAX_THREADS 3
#define BUF_SIZE 255

DWORD WINAPI checkFileByDir(LPVOID lpParam);
void ErrorHandler(LPTSTR lpszFunction);

typedef struct MyData {
	int val1;
	int val2;
} MYDATA, *PMYDATA;

typedef struct FileÝnfo {
	char *dirname;
} myFile, *PmyFile;


void start()
{
	PmyFile pDataArray[MAX_THREADS];
	DWORD   dwThreadIdArray[MAX_THREADS];
	HANDLE  hThreadArray[MAX_THREADS];
	int i = 0;

	pDataArray[i] = (PmyFile)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(myFile));

	pDataArray[i]->dirname = "c:\\";
	hThreadArray[i] = CreateThread(
		NULL,                   // default security attributes
		0,                      // use default stack size  
		checkFileByDir,       // thread function name
		pDataArray[i],          // argument to thread function 
		0,                      // use default creation flags 
		&dwThreadIdArray[i]);
	i++;

	WaitForMultipleObjects(MAX_THREADS, hThreadArray, TRUE, INFINITE);
}
int _tmain()
{
	
	start();

	/*for (int i = 0; i<MAX_THREADS; i++)
	{
		CloseHandle(hThreadArray[i]);
		if (pDataArray[i] != NULL)
		{
			HeapFree(GetProcessHeap(), 0, pDataArray[i]);
			pDataArray[i] = NULL;  
		}
	}*/

	return 0;
}


DWORD WINAPI mythread(LPVOID lpParam)
{
	HANDLE hStdout;
	PMYDATA pDataArray;

	TCHAR msgBuf[BUF_SIZE];
	size_t cchStringSize;
	DWORD dwChars;

	// Make sure there is a console to receive output results. 

	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdout == INVALID_HANDLE_VALUE)
		return 1;

	// Cast the parameter to the correct data type.
	// The pointer is known to be valid because 
	// it was checked for NULL before the thread was created.

	pDataArray = (PMYDATA)lpParam;

	// Print the parameter values using thread-safe functions.

	StringCchPrintf(msgBuf, BUF_SIZE, TEXT("Parameters = %d, %d\n"),
		pDataArray->val1, pDataArray->val2);
	StringCchLength(msgBuf, BUF_SIZE, &cchStringSize);
	WriteConsole(hStdout, msgBuf, (DWORD)cchStringSize, &dwChars, NULL);

	return 0;
}

DWORD WINAPI checkFileByDir(LPVOID lpParam)
{
	DIR *dir;
	PmyFile pmyFile = (PmyFile)lpParam;;
	struct dirent *ent;
	char *kstr;
	bool flag = false;
	if ((dir = opendir(pmyFile->dirname)) != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			char * name = ent->d_name;
			if (strstr(name, "deplister") != NULL)
			{
				printf("%s\n", ent->d_name);
				printf("yes find\n");
				printf("%s\n", pmyFile->dirname);
				flag = true;
				kstr = "0";
				break;
			}
			else
			{
				char *cstr = pmyFile->dirname;
				strcat(cstr, "\\");
				strcat(cstr, ent->d_name);
				kstr = cstr;
			}
		}
	}
	return 0;
}

void ErrorHandler(LPTSTR lpszFunction)
{
		// Retrieve the system error message for the last-error code.

		LPVOID lpMsgBuf;
		LPVOID lpDisplayBuf;
		DWORD dw = GetLastError();

		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dw,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf,
			0, NULL);

		// Display the error message.

		lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
			(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
		StringCchPrintf((LPTSTR)lpDisplayBuf,
			LocalSize(lpDisplayBuf) / sizeof(TCHAR),
			TEXT("%s failed with error %d: %s"),
			lpszFunction, dw, lpMsgBuf);
		MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

		// Free error-handling buffer allocations.

		LocalFree(lpMsgBuf);
		LocalFree(lpDisplayBuf);
	
}