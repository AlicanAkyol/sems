/*Anubis Sandbox detection tool
* v1.0
*/

#include "Func.h"

void computerNameAnubis()
{
	if (getComputerName() == (TCHAR*)"USER")
	{
		createAndWriteFile("anubis.txt");
		printf("Anubis Detected \n");
	}
}

void sandboxieDetect()
{
	if (GetModuleHandle((LPCWSTR)"sbiedll.dll") != NULL) {
		createAndWriteFile("sandboxie.txt");
		printf("Sandboxie Detected \n");
	}
}

void anubisDetect()
{
	computerNameAnubis();
	sandboxieDetect();
}