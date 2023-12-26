#if defined(__OpenBSD__) || defined(__NetBSD__) || defined(__FreeBSD__)
	#include <sys/types.h>
	#include <sys/sysctl.h>
#else
	#include <sys/sysinfo.h>
#endif

char* fetchCpuThreads(void) {
#if defined(__OpenBSD__) || defined(__NetBSD__) || defined(__FreeBSD__)
	int cpuThreads_mib[2];
	size_t cpuThreads_len;
	int ncpu;

	cpuThreads_mib[0] = CTL_HW;
#ifdef __OpenBSD__
	cpuThreads_mib[1] = HW_NCPUFOUND;
#else
	cpuThreads_mib[1] = HW_NCPU;
#endif

	cpuThreads_len = sizeof(ncpu);

	if (sysctl(cpuThreads_mib, 2, &ncpu, &cpuThreads_len, NULL, 0) == -1) {
		perror("sysctl");
		exit(EXIT_FAILURE);
	}
#else
	int ncpu = get_nprocs();
#endif
	int cpuThreadsCSize = snprintf(NULL, 0, "CPU Cores: %d\n", ncpu + 1);
	char *cpuThreads = (char *)malloc(cpuThreadsCSize);
	sprintf(cpuThreads, "CPU Cores: %d\n", ncpu);

	return cpuThreads;
}
