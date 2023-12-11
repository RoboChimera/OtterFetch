#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <sys/utsname.h>

#if defined(__OpenBSD__) || defined(__NetBSD__) || defined(__FreeBSD__)
	#include <sys/types.h>
	#include <sys/sysctl.h>
#else
	#include <sys/sysinfo.h>
#endif

void fetch(GtkWidget *label) {
	struct utsname unamePointer;
	uname(&unamePointer);

#if defined(__OpenBSD__) || defined(__NetBSD__) || defined(__FreeBSD__)
	int totalram_mib[2];
	size_t totalram_len;
	uint64_t totalramUint;

	// Get the physical RAM size
	totalram_mib[0] = CTL_HW;
	totalram_mib[1] = HW_PHYSMEM64;

	totalram_len = sizeof(totalramUint);
	if (sysctl(totalram_mib, 2, &totalramUint, &totalram_len, NULL, 0) == -1) {
		perror("sysctl");
		exit(EXIT_FAILURE);
	}

	int freeram_mib[2];
	size_t freeram_len;
	uint64_t freeramUint;

	// Get free RAM size
	freeram_mib[0] = CTL_HW;
	freeram_mib[1] = HW_USERMEM64;

	freeram_len = sizeof(freeramUint);
	if (sysctl(freeram_mib, 2, &freeramUint, &freeram_len, NULL, 0) == -1) {
		perror("sysctl");
		exit(EXIT_FAILURE);
	}

	float totalramValue = totalramUint / 1000 / 1000;
	float freeramValue = freeramUint / 1000 / 1000;

	int versionCSize = snprintf(NULL, 0, "Version: %s %s\n", unamePointer.sysname, unamePointer.release) + 1;
	int totalramCSize = snprintf(NULL, 0, "Built-in memory: %.1f MB\n", totalramValue) + 1;
	int freeramCSize = snprintf(NULL, 0, "Available Memory: %.1f MB\n", freeramValue) + 1;
#else
	struct sysinfo sInfo;
	sysinfo(&sInfo);

	//Calculate the size needed for the strings
	int versionCSize = snprintf(NULL, 0, "Version: %s %s\n", unamePointer.sysname, unamePointer.release) + 1;
	int totalramCSize = snprintf(NULL, 0, "Built-in memory: %.1f MB\n", sInfo.totalram / 1000.0 / 1000.0) + 1;
	int freeramCSize = snprintf(NULL, 0, "Available Memory: %.1f MB\n", sInfo.freeram / 1000.0 / 1000.0) + 1;
#endif
	// Allocate memory for the strings
	char *version = (char *)malloc(versionCSize);
	char *totalram = (char *)malloc(totalramCSize);
	char *freeram = (char *)malloc(freeramCSize);

#if defined(__OpenBSD__) || defined(__NetBSD__) || defined(__FreeBSD__)
	// Concatenate the string
	sprintf(version, "Version: %s %s\n", unamePointer.sysname, unamePointer.release);
	sprintf(totalram, "Built-in memory: %.1f MB\n", totalramValue);
	sprintf(freeram, "Available memory: %.1f MB\n", freeramValue);

#else
	sprintf(version, "Version: %s %s\n", unamePointer.sysname, unamePointer.release);
	sprintf(totalram, "Built-in memory: %.1f MB\n", sInfo.totalram / 1000.0 / 1000.0);
	sprintf(freeram, "Available memory: %.1f MB\n", sInfo.freeram / 1000.0 / 1000.0);
#endif


	// Update the GTK label
	char *displayText = g_strdup_printf("%s%sVirt Memory: Off\n%s", version, totalram, freeram);
	gtk_label_set_text(GTK_LABEL(label), displayText);

	free(version);
	free(totalram);
	free(freeram);
	g_free(displayText);
}

int main(int argc, char *argv[]) {
	GtkWidget *window, *grid;
	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "OtterFetch");
	gtk_window_set_default_size(GTK_WINDOW(window), 500, 250);
	gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

	// Making the grid
	grid = gtk_grid_new();
	gtk_container_add(GTK_CONTAINER(window), grid);

	// Making the labels
	GtkWidget *label = gtk_label_new("");
	fetch(label);
	gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
	gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);

	gtk_widget_show_all(window);
	gtk_main();

	return 0;
}
