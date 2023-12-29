#ifdef __linux__
	#include <sys/sysinfo.h>
#elif __sun
	#include <kstat.h>
#else
	#include <sys/types.h>
	#include <sys/sysctl.h>
#endif

char* fetchFreeram(void) {
#ifdef __linux__
	struct sysinfo sInfo;
	sysinfo(&sInfo);

	float freeramValue = sInfo.freeram / 1000 / 1000;
#elif __sun
	freeramKstat_ctl_t *freeramKctl;
	freeramKstat_t *freeramKstat;

	if ((freeramKctl = freeramKstat_open()) == NULL) {
		perror("freeramKstat_open");
	}

	freeramKstat = freeramKstat_lookup(freeramKctl, "unix", 0, "system_pages");
	if (freeramKstat == NULL) {
		perror("freeramKstat_lookup");
		freeramKstat_close(freeramKctl);
	}

	if (freeramKstat_read(freeramKctl, freeramKstat, NULL) == -1) {
		perror("freeramKstat_read");
		freeramKstat_close(freeramKctl);
	}

	freeramKstat_named_t *freeramKname = freeramKstat_data_lookup(freeramKstat, "availrmem");
	if (freeramKname == NULL) {
		perror("freeramKstat_data_lookup");
		freeramKstat_close(freeramKctl);
	}

	uint64_t freeramUint = freeramKname->value.i64;
	float freeramValue = freeramUint / 1024;
#else
	int freeram_mib[2];
	size_t freeram_len;
	uint64_t freeramUint;

	// Get free RAM size
	freeram_mib[0] = CTL_HW;
#ifdef __OpenBSD__
	freeram_mib[1] = HW_USERMEM64;
#else
	freeram_mib[1] = HW_USERMEM;
	float freeramValue = freeramUint / 1000 / 1000;
#endif


	freeram_len = sizeof(freeramUint);
	if (sysctl(freeram_mib, 2, &freeramUint, &freeram_len, NULL, 0) == -1) {
		perror("sysctl");
		exit(EXIT_FAILURE);
	}

	float freeramValue = freeramUint / 1000 / 1000;

#endif
	int freeramCSize = snprintf(NULL, 0, "Available Memory: %.1f MB\n", freeramValue) + 1;
	char *freeram = (char *)malloc(freeramCSize);
	sprintf(freeram, "Available memory: %.1f MB\n", freeramValue);

	return freeram;
}
