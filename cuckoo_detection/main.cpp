/* VirtualBox, VMWare, Threat Expert, Anubis and Cuckoo Sandbox detection tool.
 * v1.0
*/

#include "Func.h"

int main()
{
	anubisDetect();
	threatExpertDetect();
	cuckooDetect();
	virtualMachineDetect();
	virtualBoxDetect();	
	getchar();
}
