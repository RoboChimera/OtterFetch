#if defined(__OpenBSD__) || defined(__NetBSD__) || defined(__FreeBSD__)
	#include <sys/types.h>
	#include <sys/sysctl.h>
#else
	#include <sys/sysinfo.h>
#endif

char* fetchTotalram(void) {
#if defined(__OpenBSD__) || defined(__NetBSD__) || defined(__FreeBSD__)
	int totalram_mib[2];
	size_t totalramLen;
	uint64_t totalramUint;

	// Get the physical RAM size
	totalram_mib[0] = CTL_HW;
#ifdef __OpenBSD__
	totalram_mib[1] = HW_PHYSMEM64;
#else
	totalram_mib[1] = HW_PHYSMEM;
#endif

	totalramLen = sizeof(totalramUint);
	if (sysctl(totalram_mib, 2, &totalramUint, &totalramLen, NULL, 0) == -1) {
		perror("sysctl");
		exit(EXIT_FAILURE);
	}

	float totalramValue = totalramUint / 1000 / 1000;
#else
	struct sysinfo sInfo;
	sysinfo(&sInfo);

	float totalramValue = sInfo.totalram / 1000 / 1000;
#endif
	int totalramCSize = snprintf(NULL, 0, "Built-in memory: %.1f MB\n", totalramValue) + 1;
	char *totalram = (char *)malloc(totalramCSize);
	sprintf(totalram, "Built-in memory: %.1f MB\n", totalramValue);

	return totalram;
}