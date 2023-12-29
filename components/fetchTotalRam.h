#include <unistd.h>

char* fetchTotalram(void) {
	long pages = sysconf(_SC_PHYS_PAGES);
	long pageSize = sysconf(_SC_PAGE_SIZE);
	long long physRam = pages * pageSize;

	float totalramValue = physRam / 1024 / 1024;

	int totalramCSize = snprintf(NULL, 0, "Built-in memory: %.1f MB\n", totalramValue) + 1;
	char *totalram = (char *)malloc(totalramCSize);
	sprintf(totalram, "Built-in memory: %.1f MB\n", totalramValue);

	return totalram;
}
