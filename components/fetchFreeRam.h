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
	kstat_ctl_t *kctl;
	kstat_t *kstat;

	if ((kctl = kstat_open()) == NULL) {
		perror("kstat_open");
	}

	kstat = kstat_lookup(kctl, "unix", 0, "system_pages");
	if (kstat == NULL) {
		perror("kstat_lookup");
		kstat_close(kctl);
	}

	if (kstat_read(kctl, kstat, NULL) == -1) {
		perror("kstat_read");
		kstat_close(kctl);
	}

	kstat_named_t *freeramKname = kstat_data_lookup(kstat, "availrmem");
	if (freeramKname == NULL) {
		perror("kstat_data_lookup");
		kstat_close(kctl);
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
