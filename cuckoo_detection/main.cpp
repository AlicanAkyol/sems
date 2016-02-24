/* VirtualBox, VMWare, Analysis Tools, Threat Expert, Anubis, Cuckoo Sandbox and some analysis environments detection tool.
 * v1.0
*/

#include "Func.h"

int main()
{
	someSandboxesDetect();
	cuckooDetect();
	virtualMachineDetect();
	virtualBoxDetect();
	analysisToolsDetect();
	control();
	getchar();
}
