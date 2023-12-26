#if defined(__OpenBSD__) || defined(__NetBSD__) || defined(__FreeBSD__)
	#include <sys/types.h>
	#include <sys/sysctl.h>
#else
	#include <sys/sysinfo.h>
#endif

char* fetchCpuThreads(void) {
#if defined(__OpenBSD__) || defined(__NetBSD__) || defined(__FreeBSD__)
#else
	int cpuThreadsCSize = snprintf(NULL, 0, "CPU Cores: %d\n", get_nprocs()) + 1;
	char *cpuThreads = (char *)malloc(cpuThreadsCSize);
	sprintf(cpuThreads, "CPU Cores: %d\n", get_nprocs());
#endif
	return cpuThreads;
}
