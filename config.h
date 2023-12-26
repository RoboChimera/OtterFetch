/*
 * function description		function name		library directory/file		notes
 *
 * OS + Version			fetchUname()		components/fetchUname.h
 *
 * Total memory in GB		fetchTotalram()		components/fetchTotalRam.h
 * Free memory in GB		fetchFreeram()		components/fetchUname.h
 *
 * Available Threads		fetchCpuThreads()	components/fetchCpuThreads.h	Function works only on linux right now
*/

// Include only needed libraries
#include "components/fetchTotalRam.h"
#include "components/fetchFreeRam.h"
#include "components/fetchUname.h"
#include "components/fetchCpuThreads.h"

void fetch(GtkWidget *label) {
	char *totalram = fetchTotalram();
	char *freeram = fetchFreeram();
	char *version = fetchUname();
	char *cputhreads = fetchCpuThreads();

	// Configurates the GTK label
	char *displayText = g_strdup_printf("%s%s%s%s", version, totalram, freeram, cputhreads);
	gtk_label_set_text(GTK_LABEL(label), displayText);

	// Frees variables when no longer needed
	free(version);
	free(totalram);
	free(freeram);
	free(cputhreads);
	g_free(displayText);
}
