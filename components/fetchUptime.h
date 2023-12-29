#ifdef __linux__
	#include <sys/sysinfo.h>
#else
	#include <sys/types.h>
	#include <sys/sysctl.h>
#endif

char* fetchUptime(void) {
#ifdef __linux__
	struct sysinfo sInfo;
	sysinfo(&sInfo);

	int second = sInfo.uptime;
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
