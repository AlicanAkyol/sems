/*VirtualMachine detection tool
* Resources
*  -ScoopyNG
*  -http://www.offensivecomputing.net/ Written by Danny Quist, Offensive Computing
*/

#include "Func.h"

void idtr()
{
	unsigned char idtr[6];
	_asm sidt idtr
	unsigned int idt_base = 0;

	idt_base = *((unsigned long *)&idtr[2]);

	if ((idt_base >> 24) == 0xff) {
		createAndWriteFile("idtr.txt");
		printf("VM Detected (%s)\n", "idtr");
		return;
	}
}

void ldtr()
{
	unsigned char   ldtr[5] = "\xef\xbe\xad\xde";
	unsigned long   ldt = 0;

	_asm sldt ldtr
	ldt = *((unsigned long *)&ldtr[0]);

	if (ldt != 0xdead0000)
	{
		createAndWriteFile("ldtr.txt");
		printf("VM Detected (%s)\n", "ldtr");
	}
}

void gdtr()
{
	unsigned char   gdtr[6];
	unsigned long   gdt = 0;

	_asm sgdt gdtr
	gdt = *((unsigned long *)&gdtr[2]);

	if ((gdt >> 24) == 0xff)
	{
		createAndWriteFile("gdtr.txt");
		printf("VM Detected (%s)\n", "gdtr");
	}
}

void tr()
{
	unsigned char mem[4] = { 0, 0, 0, 0 };

	__asm str mem;

	if ((mem[0] == 0x00) && (mem[1] == 0x40))
	{
		createAndWriteFile("tr.txt");
		printf("VM Detected (%s)\n", "tr");
	}
}

void smsw()
{
	unsigned int reax = 0;

	__asm
	{
		mov eax, 0xCCCCCCCC;
		smsw eax;
		mov DWORD PTR[reax], eax;
	}

	if ((((reax >> 24) & 0xFF) == 0xcc) && (((reax >> 16) & 0xFF) == 0xcc))
	{
		createAndWriteFile("smsw.txt");
		printf("VM Detected (%s)\n", "smsw");
	}
}

void IsInsideVMWare()
{
	bool rc = true;
	__try
	{
		__asm
		{
			push   edx
				push   ecx
				push   ebx

				mov    eax, 'VMXh'
				mov    ebx, 0 // any value but not the MAGIC VALUE
				mov    ecx, 10 // get VMWare version
				mov    edx, 'VX' // port number

				in     eax, dx // read port
				// on return EAX returns the VERSION
				cmp    ebx, 'VMXh' // is it a reply from VMWare?
				setz[rc] // set return value

				pop    ebx
				pop    ecx
				pop    edx
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		rc = false;
	}

	if (rc)
	{
		createAndWriteFile("vmware.txt");
		printf("VM Detected- VMX -\n");
	}
		
}

unsigned vmware(void)
{
	try
	{
		__asm{
				mov eax, 0x564d5868
				mov cl, 0xa
				mov dx, 0x5658
				in eax, dx
				cmp ebx, 0
				jne matrix
				xor eax, eax
				ret
				matrix :
				mov eax, 1};
	}
	catch (int e)
	{
		perror("ERROR");
	}	
}

void vmFile()
{
	string stringVmFiles[20] =
	{
		"C:\\WINDOWS\\system32\\vm3dgl64.dll",
		"C:\\WINDOWS\\system32\\vm3dgl.dll",
		"C:\\WINDOWS\\system32\\vm3dum64.dll",
		"C:\\WINDOWS\\system32\\vm3dum.dll",
		"C:\\WINDOWS\\system32\\VmbuxCoinstaller.dll",
		"C:\\WINDOWS\\system32\\vmGuestLib.dll",
		"C:\\WINDOWS\\system32\\vmGuestLibJava.dll",
		"C:\\WINDOWS\\system32\\vmhgfs.dll",
		"C:\\WINDOWS\\system32\\vmwogl32.dll",
		"C:\\WINDOWS\\system32\\vmmreg32.dll",
		"C:\\WINDOWS\\system32\\vmx_fb.dll",
		"C:\\WINDOWS\\system32\\vmx_mode.dll",
		"C:\\WINDOWS\\system32\\VMUpgradeAtShutdownWXP.dll"
	};

	try
	{
		for (size_t i = 0; i < sizeof(stringVmFiles) / sizeof(stringVmFiles[0]); i++)
		{
			if (fileExist((char*)stringVmFiles[i].c_str()) != INVALID_FILE_ATTRIBUTES)
			{
				createAndWriteFile("vmfile.txt");
				printf("VM Detected (%s)\n", stringVmFiles[i].c_str());
			}	
		}
	}
	catch (int e){
		perror("ERROR");
	}	
}

void vmRegVal()
{	
	string stringVmRegVals[25][4] =
	{ 
		{ "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", "VMware User Process", "", "" },
		{ "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Setup\\SharedDlls", "C:\\WINDOWS\\system32\\VMUpgradeAtShutdownWXP.dll", "", "" },
		{ "SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E968-E325-11CE-BFC1-08002BE10318}\\0000",
		"DriverDesc", "vmware svga ii", "" },
		{ "SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E968-E325-11CE-BFC1-08002BE10318}\\0000",
		"DriverDesc", "vmware svga 3d", "" },
		{ "SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E96F-E325-11CE-BFC1-08002BE10318}\\0000",
		"InfSection", "vmmouse", "" },
		{ "SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E968-E325-11CE-BFC1-08002BE10318}\\0000",
		"DriverDesc", "vmware vmscsi controller", "" },
		{ "SYSTEM\\CurrentControlSet\\Control\\Video\\{4BEF3D64-1F2B-4026-9EE4-B6D8CD9FEA1B}\\0000",
		"Device Description", "vmware svga ii", "" },
		{ "SYSTEM\\CurrentControlSet\\Control\\Video\\{3A8088C5-4419-4572-801C-A10BA858952F}\\0000",
		"Device Description", "vmware svga 3d", "" }, 
		{ "SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E96F-E325-11CE-BFC1-08002BE10318}\\0000", 
		"DriverDesc", "VMware Pointing Device", "" },
		{ "SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E96F-E325-11CE-BFC1-08002BE10318}\\0000",
		"ProviderName", "VMware, Inc.", "" },
		{ "SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E96F-E325-11CE-BFC1-08002BE10318}\\0001",
		"DriverDesc", "VMware USB Pointing Device", "" },
		{ "SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E96F-E325-11CE-BFC1-08002BE10318}\\0001",
		"InfSection", "VMUsbMouse", "" },
		{ "SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E96F-E325-11CE-BFC1-08002BE10318}\\0001",
		"ProviderName", "VMware, Inc.", "" },
		{ "SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E968-E325-11CE-BFC1-08002BE10318}\\0000", 
		"HardwareInformation.AdapterString", "VMware SVGA 3D", "" }, 
		{ "SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E968-E325-11CE-BFC1-08002BE10318}\\0000", 
		"HardwareInformation.ChipType", "VMware Virtual SVGA 3D Graphics Adapter", "" }, 
		{ "SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E968-E325-11CE-BFC1-08002BE10318}\\0000", 
		"InfSection", "VM3D_AMD64", "" }, 
		{ "SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E968-E325-11CE-BFC1-08002BE10318}\\0000", 
		"InstalledDisplayDrivers", "vm3dum64", "" }, 
		{ "SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E968-E325-11CE-BFC1-08002BE10318}\\0000", 
		"InstalledDisplayDrivers", "vm3dum", "" }, 
		{ "SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E968-E325-11CE-BFC1-08002BE10318}\\0000", 
		"InstalledDisplayDrivers", "vm3dgl64", "" }, 
		{ "SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E968-E325-11CE-BFC1-08002BE10318}\\0000", 
		"InstalledDisplayDrivers", "vm3dgl", "" }, 
		{ "SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E968-E325-11CE-BFC1-08002BE10318}\\0000", 
		"OpenGLDriverName", "vm3dgl64.dll", "" }, 
		{ "SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E968-E325-11CE-BFC1-08002BE10318}\\0000", 
		"OpenGLDriverNameWow", "vm3dgl.dll", "" }, 
		{ "SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E968-E325-11CE-BFC1-08002BE10318}\\0000", 
		"ProviderName", "VMware, Inc.", "" }, 
		{ "SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E968-E325-11CE-BFC1-08002BE10318}\\0000", 
		"UserModeDriverName", "vm3dum64.dll", "" }, 
		{ "SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E968-E325-11CE-BFC1-08002BE10318}\\0000", 
		"UserModeDriverNameWow", "vm3dum.dll", "" }
	};

	string val1[] = { "Scsi Port 0", "Scsi Port 1", "Scsi Port 2", "Scsi Port 3", "Scsi Port 4" };
	string val2[] = { "Scsi Bus 0", "Scsi Bus 1", "Scsi Bus 2", "Scsi Bus 3", "Scsi Bus 4", "Scsi Bus 5", "Scsi Bus 6" };
	try
	{
		for (size_t i = 0; i < sizeof(val1) / sizeof(val1[0]); i++)
		{
			for (size_t j = 0; j < sizeof(val2) / sizeof(val2[0]); j++)
			{
				string str;
				str = "HARDWARE\\DEVICEMAP\\Scsi\\" + val1[i] + "\\" + val2[j] + 
					"\\Target Id 0\\Logical Unit Id 0";
				if (CheckReg(str, "Identifier", "vmware", ""))
				{
					createAndWriteFile("vmdetectedIdentifier.txt");
					printf("VM Detected (%s)\n", "Identifier");
				}

				if (CheckReg(str, "Identifier", "QEMU", ""))
				{
					createAndWriteFile("QEMU.txt");
					printf("QEMU Detected (%s)\n", "Identifier");
				}

				str = "HARDWARE\\Description\\System";
				if (CheckReg(str, "SystemBiosVersion", "QEMU", ""))
				{
					createAndWriteFile("QEMU.txt");
					printf("QEMU Detected (%s)\n", "SystemBiosVersion");
				}	
			}
		}
	}
	catch (int e){
		perror("ERROR");
	}
	try
	{
		for (size_t i = 0; i < sizeof(stringVmRegVals) / sizeof(stringVmRegVals[0]); i++)
		{
			if (CheckReg(stringVmRegVals[i][0], stringVmRegVals[i][1], stringVmRegVals[i][2], 
				stringVmRegVals[i][3]))
			{
				createAndWriteFile("vmdetected.txt");
				printf("VM Detected (%s)\n", stringVmRegVals[i][1].c_str());
			}	
		}
	}
	catch (int e){
		perror("ERROR");
	}
}

void vmRegKey()
{
	string stringVmRegKeys[19] =
	{
		"SOFTWARE\\Clients\\StartMenuInternet\\VMWAREHOSTOPEN.EXE",
		"SOFTWARE\\VMware, Inc.\\VMware Tools",
		"SOFTWARE\\Microsoft\\ESENT\\Process\\vmtoolsd",
		"SYSTEM\\CurrentControlSet\\Enum\\IDE\\CdRomNECVMWar_VMware_SATA_CD01_______________1.00____",
		"SYSTEM\\CurrentControlSet\\Enum\\IDE\\CdRomNECVMWar_VMware_IDE_CDR10_______________1.00____",
		"SYSTEM\\CurrentControlSet\\Enum\\SCSI\\Disk&Ven_VMware_&Prod_VMware_Virtual_S&Rev_1.0",
		"SYSTEM\\CurrentControlSet\\Enum\\SCSI\\Disk&Ven_VMware_&Prod_VMware_Virtual_S",
		"SYSTEM\\CurrentControlSet\\Control\\CriticalDeviceDatabase\\root#vmwvmcihostdev",
		"SYSTEM\\CurrentControlSet\\Control\\VirtualDeviceDrivers",
		"SYSTEM\\CurrentControlSet\\Services\\IRIS5",
		"SOFTWARE\\eEye Digital Security",
		"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Wireshark",
		"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\wireshark.exe",
		"SOFTWARE\\ZxSniffer.exe",
		"SOFTWARE\\Cygwin",
		"SOFTWARE\\B Labs\\Bopup Observer",
		"AppEvents\\Schemes\\Apps\\Bopup Observer",
		"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Win Sniffer_is1",
		"SOFTWARE\\Win Sniffer"
	};

	try
	{
		for (size_t i = 0; i < sizeof(stringVmRegKeys) / sizeof(stringVmRegKeys[0]); i++)
		{
			if (registerOpenKey((char *)stringVmRegKeys[i].c_str()) == ERROR_SUCCESS)
			{
				createAndWriteFile("vmregKey.txt");
				printf("Analysis environment detected (%s)\n", stringVmRegKeys[i].c_str());
			}		
		}
	}
	catch (int e){
		perror("ERROR");
	}	
}

void vm56()
{
	if (vmware())
	{
		createAndWriteFile("0564d5868.txt");
		printf("VM Detected - 0x564d5868 -\n");
	}
		
}

void runningProcess()
{
	string names[2] = { "vmtoolsd", "vmacthlp" };
	try
	{
		for (size_t i = 0; i < sizeof(names) / sizeof(names[0]); i++)
		{
			if (processTools(names[i], 8))
			{
				createAndWriteFile("runningProcess.txt");
				printf("VM Detected (%s)\n", names[i].c_str());
			}
				
		}
	}
	catch(int e){
		perror("ERROR");
	}	
}

void runningServices()
{
	wchar_t* services[11] = { L"vmhgfs", L"VMMEMCTL", L"vmmouse", L"vmrawdsk",
		L"VMTools", L"vmusbmouse", L"vmvss", L"vmscsi", L"VMware Physical Disk Helper Service",
		L"vmxnet", L"vmx_svga" };
	try
	{
		for (size_t i = 0; i < sizeof(services) / sizeof(services[0]); i++)
		{
			if (servicesTools(services[i]))
			{
				createAndWriteFile("runningServices.txt");
				wprintf(L"VM Detected (%s)\n", services[i]);
			}	
		}
	}
	catch (int e){
		perror("ERROR");
	}	
}

void memory()
{
	unsigned int	a = 0;
	__try {
		__asm {

			// save register values on the stack
				push eax
				push ebx
				push ecx
				push edx

				// perform fingerprint
				mov eax, 'VMXh'		// VMware magic value (0x564D5868)
				mov ecx, 14h		// get memory size command (0x14)
				mov dx, 'VX'		// special VMware I/O port (0x5658)

				in eax, dx			// special I/O cmd

				mov a, eax			// data 

				// restore register values from the stack
				pop edx
				pop ecx
				pop ebx
				pop eax
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {}

	if (a > 0)
	{
		createAndWriteFile("vmmemory.txt");
		wprintf(L"VM Detected (memory)\n");
	}
}

void version()
{
	unsigned int	a, b;

	__try {
		__asm {

			// save register values on the stack
				push eax
				push ebx
				push ecx
				push edx

				// perform fingerprint
				mov eax, 'VMXh'	// VMware magic value (0x564D5868)
				mov ecx, 0Ah		// special version cmd (0x0a)
				mov dx, 'VX'		// special VMware I/O port (0x5658)

				in eax, dx			// special I/O cmd

				mov a, ebx			// data 
				mov b, ecx			// data	(eax gets also modified but will not be evaluated)

				// restore register values from the stack
				pop edx
				pop ecx
				pop ebx
				pop eax
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {}

	

	if (a == 'VMXh') 		// is the value equal to the VMware magic value?
	{
		createAndWriteFile("vmversion.txt");
		wprintf(L"VM Detected (version)\n");
	}
		
}

void vmMac()
{
	try
	{
		char *mac = strtok(Mac(), ":");
		if (strcmp(mac, "00") == 0)
		{
			mac = strtok(NULL, ":");
			if (strcmp(mac, "0c") == 0)
			{
				mac = strtok(NULL, ":");
				if (strcmp(mac, "29") == 0)
				{
					createAndWriteFile("vmmac.txt");

					printf("VMWare Detected (MAC)\n");
				}
			}
		}
	}
	catch (int e){
	}
}

void virtualMachineDetect()
{
	try
	{
		idtr();
	}
	catch (int e){}
	try
	{
		ldtr();
	}
	catch (int e){}
	try
	{
		gdtr();
	}
	catch (int e){}
	try
	{
		tr();
	}
	catch (int e){}
	try
	{
		smsw();
	}
	catch (int e){}
	try
	{
		runningProcess();
	}
	catch (int e){}
	try
	{
		runningServices();
	}
	catch (int e){}
	try
	{
		vmRegVal();
	}
	catch (int e){}
	try
	{
		vmRegKey();
	}
	catch (int e){}
	try
	{
		vmFile();
	}
	catch (int e){}
	try
	{
		IsInsideVMWare();
	}
	catch (int e){}
	try
	{
		memory();
	}
	catch (int e){}
	try
	{
		version();
	}
	catch (int e){}
	try
	{
		vmMac();
	}
	catch (int e){}

}
