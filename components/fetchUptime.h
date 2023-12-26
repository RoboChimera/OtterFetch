#if defined(__OpenBSD__) || defined(__NetBSD__) || defined(__FreeBSD__)
	#include <sys/types.h>
	#include <sys/sysctl.h>
#else
	#include <sys/sysinfo.h>
#endif

char* fetchUptime(void) {
#if defined(__OpenBSD__) || defined(__NetBSD__) || defined(__FreeBSD__)
#else
	struct sysinfo sInfo;
	sysinfo(&sInfo);

	int second = sInfo.uptime;
	int hour = 0, minute = 0;
	
	while (second >= 3600) {
		second = second - 3600;
		hour++;
	}

	while (second >= 60) {
		second = second - 60;
		minute++;
	}

#endif

	int uptimeCSize = snprintf(NULL, 0, "Uptime: %dh %dm %ds\n", hour, minute, second + 1);
	char *uptime = (char *)malloc(uptimeCSize);
	sprintf(uptime, "Uptime: %dh %dm %ds\n", hour, minute, second);

	return uptime;
}
