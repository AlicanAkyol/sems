/*Analysis tools detection tool
* v1.0
*/

#include "Func.h"

void immunityDetect()
{
	if (processTools("Immunity", 8))
	{
		createAndWriteFile("immunityDetect.txt");
		printf("immunity detect \n");
	}
}

void processHackerDetect()
{
	if (processTools("ProcessHacker", 13))
	{
		createAndWriteFile("processHackerDetect.txt");
		printf("process hacker detect \n");
	}
}

void processExplorerDetect()
{
	if (processTools("procexp", 7))
	{
		createAndWriteFile("processExplorerDetect.txt");
		printf("Process Explorer detect \n");
	}
}

void processMonitorDetect()
{
	if (processTools("procmon", 7))
	{
		createAndWriteFile("processMonitorDetect.txt");
		printf("Process Monitor detect \n");
	}
}

void idaqDetect()
{
	if (processTools("idaq", 4))
	{
		createAndWriteFile("idaDetect.txt");
		printf("ida detect \n");
	}
}

void regshotDetect()
{
	if (processTools("regshot", 7))
	{
		createAndWriteFile("regshotDetect.txt");
		printf("regshot detect \n");
	}
}

void wiresharkDetect()
{
	if (processTools("Wireshark", 9))
	{
		createAndWriteFile("WiresharkDetect.txt");
		printf("Wireshark detect \n");
	}
}

void analysisToolsDetect()
{
	immunityDetect();
	processHackerDetect();
	processExplorerDetect();
	processMonitorDetect();
	idaqDetect();
	wiresharkDetect();
	regshotDetect();
}