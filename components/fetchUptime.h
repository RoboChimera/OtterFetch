#ifdef __linux__
	#include <sys/sysinfo.h>
#elif __sun
	#include <kstat.h>
#else
	#include <sys/types.h>
	#include <sys/sysctl.h>
#endif

char* fetchUptime(void) {
#ifdef __linux__
	struct sysinfo sInfo;
	sysinfo(&sInfo);

	int second = sInfo.uptime;
#elif __sun
	kstat_ctl_t *uptimeKctl;
	kstat_t *uptimeKstat;

	if ((uptimeKctl = kstat_open()) == NULL) {
		perror("kstat_open");
	}

	uptimeKstat = kstat_lookup(uptimeKctl, "unix", 0, "system_misc");
	if (uptimeKstat == NULL) {
		perror("kstat_lookup");
		kstat_close(uptimeKctl);
	}

	if(kstat_read(uptimeKctl, uptimeKstat, NULL) == -1) {
		perror("kstat_read");
		kstat_close(uptimeKctl);
	}

	kstat_named_t *uptimeKname = kstat_data_lookup(uptimeKstat, "boot_time");
	if (uptimeKname == NULL) {
		perror("kstat_data_lookup");
		kstat_close(uptimeKctl);
	}

	uint64_t uptimeUint = uptimeKname->value.i64;
	int second = uptimeUint;
#else
	int uptime_mib[2];
	size_t uptime_len;
	struct timeval sinceBootTime;

	uptime_mib[0] = CTL_KERN;
	uptime_mib[1] = KERN_BOOTTIME;

	uptime_len = sizeof(sinceBootTime);
	if (sysctl(uptime_mib, 2, &sinceBootTime, &uptime_len, NULL, 0) == -1) {
		perror("SYSCTL");
		exit(EXIT_FAILURE);
	}

	int second = time(NULL) - sinceBootTime.tv_sec;
#endif

	int hour = 0, minute = 0;
	
	while (second >= 3600) {
		second = second - 3600;
		hour++;
	}

	while (second >= 60) {
		second = second - 60;
		minute++;
	}

	int uptimeCSize = snprintf(NULL, 0, "Uptime: %dh %dm %ds\n", hour, minute, second + 1);
	char *uptime = (char *)malloc(uptimeCSize);
	sprintf(uptime, "Uptime: %dh %dm %ds\n", hour, minute, second);

	return uptime;
}
