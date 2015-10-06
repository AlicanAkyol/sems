/*Threat Expert detection tool
* v1.0
*/

#include "Func.h"

void computerNameThreatExpert()
{
	if (getComputerName() == (TCHAR*)"COMPUTERNAME")
	{
		createAndWriteFile("threatexpert.txt");
		printf("Threat Expert Detected \n");
	}
}

void threatExpertDetect()
{
	computerNameThreatExpert();
}