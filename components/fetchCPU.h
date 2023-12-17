#if defined(__OpenBSD__) || defined(__NetBSD__) || defined(__FreeBSD__)
	#include <sys/types.h>
	#include <sys/sysctl.h>
#else
	#include <sys/sysinfo.h>
#endif

char* fetchCPU(void) {
#if defined(__OpenBSD__) || defined(__NetBSD__) || defined(__FreeBSD__)
	int cpuname_mib[2];
	size_t cpunameLen;
	char *cpuChar;

	// Get CPU
	cpu_mib[0] = CTL_HW;
	cpu_mib[1] = HW_MODEL;

	if (sysctl(cpuname_mib, 2, NULL, &cpunameLen, NULL, 0) == -1) {
		return NULL;
	}
	cpuChar = (char *)malloc(cpuLen);

	int cpuCSize = snprintf(NULL, 0, "CPU: %s\n", cpuChar) + 1;
	char *cpuName = (char *)malloc(cpuCSize);
	sprintf(cpuName, "CPU: %s\n", cpuChar);
	return cpuName;
#endif
}
