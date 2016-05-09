# Anti-Sandbox and Anti-Virtual Machine Tool 
(Turkish : https://github.com/AlicanAkyol/sems/blob/master/Readme_Turkish.md)

( Sha1 (sems.exe) : 06598E9948C2E256C871E66B5578D51A1886758F)

Modern malwares are equipped with anti-analysis techniques in order to evade analysis. It is common for malwares to check for the presence of any virtualization environment, any malware analysis sandboxes or any analysis tools during runtime. 

sems is a tool which is created to help malware researchers by checking their environments for the signatures of any virtualization techniques, malware sandbox tools or well know malware analysis tools. sems is using the same techniques and looking for the same footprints that evasive malwares do in order to detect if it is running in a controlled environment. So it is useful for malware researchers to check if the analysis environment is inevasible. 

## How it works?
##### Virtual Machine
Once the tool is run in a virtual machine(Virtualbox, Vmware, Qemu), it performs all the checks which are shown below and drops logs to the console about detected signatures until the "control" text is shown. In addition to that a separate .txt file with the finding name is created in the running directory for each detected signatures. Example; vboxBios.txt will be created for virtualbox bios signature.

##### Malware Sandbox
sems tool is sent to malware sandbox like any other malware samples and waited until the completion of analysis. Detected signatures can be seen in "File Operations" section of the sandbox report hence sems drops separate .txt files for each findings.

## Example Detection Signatures

Signatures found (files created) when sems is sent to Cuckoo Sandbox:
![alt tag](https://github.com/AlicanAkyol/sems/blob/master/cuckooResult.png)

Full report can be found in https://malwr.com/analysis/OWZmMWYzNmJkNDM3NGExMWFjODY2MGE2OWZmZjQzZjE/ (7 April 2016)

Signatures found when sems is run in VMware:
![alt tag](https://github.com/AlicanAkyol/sems/blob/master/vmware_normal.png)

Signatures found when sems is run in VirtualBox:
![alt tag](https://github.com/AlicanAkyol/sems/blob/master/Sanalla%C5%9Ft%C4%B1rmaOrtam%C4%B1TespitArac%C4%B1-2.png)


Cuckoo report can be found in : https://malwr.com/analysis/OTJmMDlhOWViMjlhNGY1MDgzNmM5ZDMzZGZlZjI2ZDg/

Threatexpert report can be found in : http://www.threatexpert.com/report.aspx?md5=b75f84ea8a08eade90e7afd499cd14e1

Comodo report can be found in : http://camas.comodo.com/cgi-bin/submit?file=8a7962a180d09fe3274c09abe4eb9182b500360cb72ef2f1070226db4c01e699

Payload Security report can be found in : https://www.hybrid-analysis.com/sample/3a5481d105673bf20256512c9a32b60e946a240c1793e2603c226c788f234055?environmentId=1


### VirtualBox Detection
  - Files
  - Regedit
  - Folder
  - Services
  - Mac
  - Bios
  - Window

### VMWare Detection
  - Files
  - Folder
  - Regedit
  - Services
  - Mac
  - Bios
  - Window
  - Magic
  - Memory
  - Version
  - IDTR, LDTR, TR, SMSW, I/O Port

### QEMU Detection
  - Regedit
  - Bios
  - CPU

### Cuckoo Sandbox Detection
  - Files
  - Folder
  - Port
  - Hooked Function
  - Core Number
  - Pipe
  - Modules

### Some Sandboxes Detection
Anubis , Thread Expert , Cuckoo , Sandboxie , CWSandbox
  - Computer Name
  - Core Number
  - Modules
  - Check internet
  - Disk spaces
  - Files

### Analysis Tools Detection
  - Immunity Debugger
  - Ollydbg
  - Ida Pro
  - Regshot
  - Fiddler
  - Wireshark
  - Process Monitor
  - Process Hacker
  - Process Explorer

**IMPORTANT NOTICE:** The project is created for educational purposes only. Curators are not taking responsibility in any illegal uses. 
