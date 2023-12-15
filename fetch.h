#include <sys/utsname.h>
#if defined(__OpenBSD__) || defined(__NetBSD__) || defined(__FreeBSD__)
	#include <sys/types.h>
	#include <sys/sysctl.h>
#else
	#include <sys/sysinfo.h>
#endif

int fetchTotalram(void) {
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
	return totalramValue;
}

int fetchFreeram(void) {
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
	return freeramValue;
}

int fetchUname(void) {
	// For another day :3
}

void fetch(GtkWidget *label) {
	struct utsname unamePointer;
	uname(&unamePointer);
	float totalramValue = fetchTotalram();
	float freeramValue = fetchFreeram();

	int versionCSize = snprintf(NULL, 0, "Version: %s %s\n", unamePointer.sysname, unamePointer.release) + 1;
	int totalramCSize = snprintf(NULL, 0, "Built-in memory: %.1f MB\n", totalramValue) + 1;
	int freeramCSize = snprintf(NULL, 0, "Available Memory: %.1f MB\n", freeramValue) + 1;

	// Allocate memory for the strings
	char *version = (char *)malloc(versionCSize);
	char *totalram = (char *)malloc(totalramCSize);
	char *freeram = (char *)malloc(freeramCSize);

	// Concatenate the string
	sprintf(version, "Version: %s %s\n", unamePointer.sysname, unamePointer.release);
	sprintf(totalram, "Built-in memory: %.1f MB\n", totalramValue);
	sprintf(freeram, "Available memory: %.1f MB\n", freeramValue);

	// Update the GTK label
	char *displayText = g_strdup_printf("%s%s%s", version, totalram, freeram);
	gtk_label_set_text(GTK_LABEL(label), displayText);

	free(version);
	free(totalram);
	free(freeram);
	g_free(displayText);
}
