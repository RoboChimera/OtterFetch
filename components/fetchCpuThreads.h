#ifdef __linux__
	#include <sys/sysinfo.h>
#elif __sun
	#include <kstat.h>
#else
	#include <sys/types.h>
	#include <sys/sysctl.h>
#endif

char* fetchCpuThreads(void) {
#ifdef __linux__
	int ncpu = get_nprocs();
#elif __sun
	kstat_ctl_t *ncpuKctl;
	kstat_t *ncpuKstat;

	if ((ncpuKctl = kstat_open()) == NULL) {
		perror("kstat_open");
	}

	ncpuKstat = kstat_lookup(ncpuKctl, "unix", 0, "system_misc");
if (ncpuKstat == NULL) {
		perror("kstat_lookup");
		kstat_close(ncpuKctl);
	}

	if (kstat_read(ncpuKctl, ncpuKstat, NULL) == -1) {
		perror("kstat_read");
		kstat_close(ncpuKctl);
	}

	kstat_named_t *ncpuKname = kstat_data_lookup(ncpuKstat, "ncpus");
	if (ncpuKname == NULL) {
		perror("kstat_data_lookup");
		kstat_close(ncpuKctl);
	}

	int ncpu = ncpuKname->value.i64;
#else
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
#endif

	int cpuThreadsCSize = snprintf(NULL, 0, "CPU Cores: %d\n", ncpu + 1);
	char *cpuThreads = (char *)malloc(cpuThreadsCSize);
	sprintf(cpuThreads, "CPU Cores: %d\n", ncpu);

	return cpuThreads;
}
