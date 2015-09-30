/*Virtualbox detection tool
 * Resources 
 *  -How to detect the Cuckoo Sandbox and hardening it ? - Olivier FERRAND
 *  -Pafish
 * v1.0
*/

#include "Func.h"

void vboxguest()
{
	if (registerOpenKey("SOFTWARE\\Oracle\\VirtualBox Guest Additions") == ERROR_SUCCESS)
	{
		printf("VirtualBox Detected (SOFTWARE\\Oracle\\VirtualBox Guest Additions)\n");
	}	
}

void vboxServices()
{
	string services[5] = { "SYSTEM\\ControlSet001\\Services\\VBoxGuest", "SYSTEM\\ControlSet001\\Services\\VBoxMouse",
		"SYSTEM\\ControlSet001\\Services\\VBoxService", "SYSTEM\\ControlSet001\\Services\\VBoxSF",
		"SYSTEM\\ControlSet001\\Services\\VBoxVideo" };
	try
	{
		for (size_t i = 0; i < sizeof(services) / sizeof(services[0]); i++)
		{
			if (registerOpenKey((char*)(services[i].c_str())) == ERROR_SUCCESS)
			{
				printf("VirtualBox Detected (%s)\n", services[i].c_str());
			}
		}
	}
	catch (int e){
		perror("ERROR");
	}	
}

void vboxFile()
{
	string files[20] = { "C:\\WINDOWS\\system32\\drivers\\VBoxMouse.sys", 
		"C:\\WINDOWS\\system32\\drivers\\VBoxGuest.sys", "C:\\WINDOWS\\system32\\drivers\\VBoxSF.sys",
		"C:\\WINDOWS\\system32\\drivers\\VBoxVideo.sys", "C:\\WINDOWS\\system32\\vboxdisp.dll", 
		"C:\\WINDOWS\\system32\\vboxhook.dll", "C:\\WINDOWS\\system32\\vboxmrxnp.dll", "C:\\WINDOWS\\system32\\vboxogl.dll", 
		"C:\\WINDOWS\\system32\\vboxoglarrayspu.dll", "C:\\WINDOWS\\system32\\vboxoglcrutil.dll", 
		"C:\\WINDOWS\\system32\\vboxoglerrorspu.dll", "C:\\WINDOWS\\system32\\vboxoglfeedbackspu.dll", 
		"C:\\WINDOWS\\system32\\vboxoglpackspu.dll", "C:\\WINDOWS\\system32\\vboxoglpassthroughspu.dll", 
		"C:\\WINDOWS\\system32\\vboxservice.exe", "C:\\WINDOWS\\system32\\vboxtray.exe",
		"C:\\WINDOWS\\system32\\VBoxControl.exe", "C:\\program files\\oracle\\virtualbox guest additions\\" };
	try
	{
		for (size_t i = 0; i < sizeof(files) / sizeof(files[0]); i++)
		{
			if (fileExist((char*)files[i].c_str()) != INVALID_FILE_ATTRIBUTES)
				printf("VirtualBox Detected (%s)\n", files[i].c_str());
		}
	}
	catch (int e){
		perror("ERROR");
	}	
}

void vboxTrayIPC()
{
	if (createFile("\\\\.\\pipe\\VBoxTrayIPC") != INVALID_HANDLE_VALUE)
	{
		printf("VirtualBox Detected (\\\\.\\pipe\\VBoxTrayIPC)\n");
	}
}

void vboxTrayTool()
{
	HWND hClass = findWindowforClass("VBoxTrayToolWndClass");
	HWND hWindow = findWindowforWindow("VBoxTrayToolWnd");
	if (hClass || hWindow)
		printf("VirtualBox Detected (TrayTool)\n");
}

void vboxShared()
{
	unsigned long pnsize = 0x1000;
	char * provider = (char *)LocalAlloc(LMEM_ZEROINIT, pnsize);
	int retv = WNetGetProviderNameA(WNNC_NET_RDR2SAMPLE, provider, &pnsize);
	if (retv == NO_ERROR)
	{
		if (lstrcmpiA(provider, "VirtualBox Shared Folders") == 0)
		{
			printf("VirtualBox detected (Shared)\n");
		}
	}
}

void vboxBios()
{
	if (CheckReg("HARDWARE\\DESCRIPTION\\System", "SystemBiosVersion", "vbox", ""))
		printf("VirtualBox Detected");
	if (CheckReg("HARDWARE\\DESCRIPTION\\System", "VideoBiosVersion", "oracle", "virtualbox"))
		printf("VirtualBox Detected");
	if (CheckReg("HARDWARE\\DEVICEMAP\\Scsi\\Scsi Port 0\\Scsi Bus 0\\Target Id 0\\Logical Unit Id 0",
		"Identifier", "vbox", ""))
		printf("VirtualBox Detected");
	if (CheckReg("SYSTEM\\CurrentControlSet\\Control\\SystemInformation", "SystemProductName", "virtualbox", ""))
		printf("VirtualBox Detected");
}

void vobxEnum()
{
	if (CheckEnum("SYSTEM\\CurrentControlSet\\Enum\\IDE", 1))
		printf("VirtualBox Detected");
}

void vobxAcpi()
{
	string reg[3] = { "HARDWARE\\ACPI\\DSDT\\VBOX__", "HARDWARE\\ACPI\\FADT\\VBOX__",
		"HARDWARE\\ACPI\\RSDT\\VBOX__" };
	try
	{
		for (size_t i = 0; i < sizeof(reg) / sizeof(reg[0]); i++)
		{
			if (registerOpenKey((char*)reg[i].c_str()) == ERROR_SUCCESS)
			{
				printf("VirtualBox Detected (%s)\n",reg[i].c_str());
			}
		}
	}
	catch (int e){
		perror("ERROR");
	}
}

void vboxDevices()
{
	string devices[4] = { "\\\\.\\VBoxMiniRdrDN", "\\\\.\\pipe\\VBoxMiniRdDN", 
		"\\\\.\\VBoxTrayIPC", "\\\\.\\pipe\\VBoxTrayIPC" };
	try
	{
		for (size_t i = 0; i < sizeof(devices) / sizeof(devices[0]); i++)
		{
			if (createFile((char*)devices[i].c_str()) != INVALID_HANDLE_VALUE)
			{
				printf("VirtualBox Detected (%s)\n", devices[i].c_str());
			}
		}
	}
	catch (int e){
		perror("ERROR");
	}	
}

void vboxMac()
{
	try
	{
		char *mac = strtok(Mac(), ":");
		if (strcmp(mac, "08") == 0)
		{
			mac = strtok(NULL, ":");
			if (strcmp(mac, "00") == 0)
			{
				mac = strtok(NULL, ":");
				if (strcmp(mac, "27") == 0)
				{
					printf("VirtualBox Detected (MAC)\n");
				}
			}
		}
	}
	catch (int e){
		perror("ERROR");
	}	
}

void virtualBoxDetect()
{
	vboxguest();
	vboxTrayIPC();
	vboxTrayTool();
	vboxShared();
	vobxAcpi();
	vobxEnum();
	vboxBios();
	vboxServices();
	vboxFile();
	vboxDevices();
	vboxMac();
}