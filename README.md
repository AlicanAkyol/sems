# Anti Sandbox and Anti Virtual Machine Tool 
(Turkish : https://github.com/AlicanAkyol/sems/blob/master/Readme_Turkish.md)

The project is maked for malware researcher. 

cuckoo_detection.exe should be run in your Virtual Machine. If any Virtual track is found, it will be shown and also .txt file will be created in your Virtual Machine like "Virtual track name.txt".

When the tool is run in Cuckoo Sandbox, result is shown below; (7 April 2016) 
(Link : https://malwr.com/analysis/OWZmMWYzNmJkNDM3NGExMWFjODY2MGE2OWZmZjQzZjE/) 

![alt tag](https://github.com/AlicanAkyol/sems/blob/master/cuckooResult.png)

Example of VMware result is shown below;
![alt tag](https://github.com/AlicanAkyol/sems/blob/master/vmware_normal.png)

Example of virtualbox result is shown below;
![alt tag](https://github.com/AlicanAkyol/sems/blob/master/Sanalla%C5%9Ft%C4%B1rmaOrtam%C4%B1TespitArac%C4%B1-2.png)


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
