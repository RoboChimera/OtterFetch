/*
 * function description		function name		library directory/file		notes
 *
 * OS + Version			fetchUname()		components/fetchUname.h
 * Uptime in Hours,		fetchUptime()		components/fetchUptime.h	Currently Only on Linux
 * Minutes and Seconds
 *
 * Total memory in GB		fetchTotalram()		components/fetchTotalRam.h
 * Free memory in GB		fetchFreeram()		components/fetchUname.h
 *
 * Available Threads		fetchCpuThreads()	components/fetchCpuThreads.h
*/

// Include only needed libraries
#include "components/fetchTotalRam.h"
#include "components/fetchFreeRam.h"
#include "components/fetchUname.h"
#include "components/fetchUptime.h"

void fetch(GtkWidget *label) {
	char *totalram = fetchTotalram();
	char *freeram = fetchFreeram();
	char *version = fetchUname();
	char *uptime = fetchUptime();

	// Configurates the GTK label
	char *displayText = g_strdup_printf("%s%s%s%s", version, totalram, freeram, uptime);
	gtk_label_set_text(GTK_LABEL(label), displayText);

	// Frees variables when no longer needed
	free(version);
	free(totalram);
	free(freeram);
	free(uptime);
	g_free(displayText);
}
