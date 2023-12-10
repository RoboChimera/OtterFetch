#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <sys/utsname.h>

#ifdef __OpenBSD__
	#include <sys/types.h>
	#include <sys/sysctl.h>
#else
	#include <sys/sysinfo.h>
#endif

/*
char* execute(const char *command) {
    FILE* bin = popen(command, "r");

    int charLength = 0;
    char* commandBuffer;

    int bufferSize = 4;
    commandBuffer = (char*)malloc(bufferSize * sizeof(char));

    char commandCBuffer;
    while ((commandCBuffer = fgetc(bin)) != EOF) {
        if (charLength == bufferSize - 1) {
            bufferSize *= 2;
            commandBuffer = (char*)realloc(commandBuffer, bufferSize * sizeof(char));
        }
        commandBuffer[charLength++] = commandCBuffer;
    }

    // Null-terminate the string
    commandBuffer[charLength] = '\0';

    // Clean up
    pclose(bin);

    return commandBuffer;
}
*/

void fetch(GtkWidget *label) {
	struct utsname unamePointer;
	uname(&unamePointer);

#ifdef __OpenBSD__
	int totalram_mib[2];
	size_t totalmem_len;
	uint64_t totalramUint;

	totalram_mib[0] = CTL_HW;
	totalram_mib[1] = HW_PHYSMEM64;

	totalmem_len = sizeof(totalramUint);

	if (sysctl(totalram_mib, 2, &totalramUint, &totalram_len, NULL, 0) == -1) {
		perror("sysctl");
		exit(EXIT_FAILURE);
	}

	int freeram_mib[2];
	size_t freeram_len;
	uint64_t freeramUint;

	freeram_mib[0] = CTL_HW;
	freeram_mib[1] = HW_USERMEM64;
	freeram_len = sizeof(freeramUint);

	if (sysctl(freeram_mib, 2, &freeramUint, &freeram_len, NULL, 0) == -1) {
		perror("sysctl");
		exit(EXIT_FAILURE);
	}

	float totalram = totalramUint / 1024 / 1024;
	float totalram = freeramUint / 1024 / 1024;
	printf("Version: %s %s\n", unamePointer.sysname, unamePointer.release);
	printf("Built-in memory: %.1f M", totalram);
	printf("Available memory: %.1f M", freeram);
#else
	struct sysinfo sInfo;
	sysinfo(&sInfo);
#endif


#ifdef __OpenBSD__
#else
	//Calculate the size needed for the strings
	int versionCSize = snprintf(NULL, 0, "Version: %s %s\n", unamePointer.sysname, unamePointer.release) + 1;
	int totalramCSize = snprintf(NULL, 0, "Built-in memory: %.1f MB\n", sInfo.totalram / 1000.0 / 1000.0) + 1;
	int freeramCSize = snprintf(NULL, 0, "Available Memory: %.1f MB\n", sInfo.freeram / 1000.0 / 1000.0) + 1;

	// Allocate memory for the strings
	char *version = (char *)malloc(versionCSize);
	char *totalram = (char *)malloc(totalramCSize);
	char *freeram = (char *)malloc(freeramCSize);

	// Concatenate the string
	sprintf(version, "Version: %s %s\n", unamePointer.sysname, unamePointer.release);
	sprintf(totalram, "Built-in memory: %.1f MB\n", sInfo.totalram / 1000.0 / 1000.0);
	sprintf(freeram, "Available memory: %.1f MB\n", sInfo.freeram / 1000.0 / 1000.0);


	// Update the GTK label
	char *displayText = g_strdup_printf("%s%sVirt Memory: Off\n%s", version, totalram, freeram);
	gtk_label_set_text(GTK_LABEL(label), displayText);

	free(version);
	free(totalram);
	free(freeram);
	g_free(displayText);
#endif
}

int main(int argc, char *argv[]) {
	GtkWidget *window, *grid;
	gtk_init(&argc, &argv);
	//gtk_init();

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
