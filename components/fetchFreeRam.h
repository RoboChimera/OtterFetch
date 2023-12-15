#if defined(__OpenBSD__) || defined(__NetBSD__) || defined(__FreeBSD__)
	#include <sys/types.h>
	#include <sys/sysctl.h>
#else
	#include <sys/sysinfo.h>
#endif

char* fetchFreeram(void) {
#if defined(__OpenBSD__) || defined(__NetBSD__) || defined(__FreeBSD__)
	int freeram_mib[2];
	size_t freeram_len;
	uint64_t freeramUint;

	// Get free RAM size
	freeram_mib[0] = CTL_HW;
#ifdef __OpenBSD__
	freeram_mib[1] = HW_USERMEM64;
#else
	freeram_mib[1] = HW_USERMEM;
#endif


	freeram_len = sizeof(freeramUint);
	if (sysctl(freeram_mib, 2, &freeramUint, &freeram_len, NULL, 0) == -1) {
		perror("sysctl");
		exit(EXIT_FAILURE);
	}

	float freeramValue = freeramUint / 1000 / 1000;

#else
	struct sysinfo sInfo;
	sysinfo(&sInfo);

	float freeramValue = sInfo.freeram / 1000 / 1000;
#endif

	int freeramCSize = snprintf(NULL, 0, "Available Memory: %.1f MB\n", freeramValue) + 1;
	char *freeram = (char *)malloc(freeramCSize);
	sprintf(freeram, "Available memory: %.1f MB\n", freeramValue);

	return freeram;
}
