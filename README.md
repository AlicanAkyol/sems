# Anti Sandbox and Anti Virtual Machine Tool

The project is maked for malware researcher. 

## How to Work it?
cuckoo_detection.exe is run any sandbox. If something is found, it is reported like vmware.txt at sandbox's result report.
If cuckoo_detection.exe is run any virtual machine, when something is found it will shown at command interpreter.

Example of virtualbox is below;


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
