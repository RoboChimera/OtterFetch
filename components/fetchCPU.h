#if defined(__OpenBSD__) || defined(__NetBSD__) || defined(__FreeBSD__)
	#include <sys/types.h>
	#include <sys/sysctl.h>
#else
	#include <sys/sysinfo.h>
#endif

char* fetchCPU(void) {
#if defined(__OpenBSD__) || defined(__NetBSD__) || defined(__FreeBSD__)
	int cpu_mib[2];
	size_t cpuLen;
	char *cpuChar;

	// Get CPU
	cpu_mib[0] = CTL_HW;
	fcpu_mib[1] = HW_MODEL;

	cpuLen = sizeof(cpuChar);

	int cpuCSize = snprintf(NULL, 0, "CPU: %s\n", cpuChar) + 1;
	char *cpuName = (char *)malloc(freeramCSize);
	sprintf(cpuName, "CPU: %s\n", cpuChar);
	return cpuName;
#endif
}
