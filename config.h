/*
 * function description		function name		library directory/file		notes		OS Compatability
 *
 * OS + Version			fetchUname()		components/fetchUname.h				Should work on most *NIX OSes without issues
 * Uptime in Hours,		fetchUptime()		components/fetchUptime.h			Works on Linux, *BSD and sunOS (tested on openIndiana, report if not working on others)
 * Minutes and Seconds
 *
 * Total memory in GB		fetchTotalram()		components/fetchTotalRam.h			Should work on most *NIX OSes without issues
 * Free memory in GB		fetchFreeram()		components/fetchUname.h				Works on Linux, *BSD and sunOS (tested on openIndiana, report if not working on others)
 *
 * Available Threads		fetchCpuThreads()	components/fetchCpuThreads.h			Works on Linux, *BSD and sunOS (tested on openIndiana, report if not working on others)
*/

// Include only needed libraries
#include "components/fetchTotalRam.h"
#include "components/fetchFreeRam.h"
#include "components/fetchUname.h"
#include "components/fetchUptime.h"
#include "components/fetchCpuThreads.h"

void fetch(GtkWidget *label) {
	char *totalram = fetchTotalram();
	char *freeram = fetchFreeram();
	char *version = fetchUname();
	char *uptime = fetchUptime();
	char *cputhreads = fetchCpuThreads();

	// Configurates the GTK label
	char *displayText = g_strdup_printf("%s%s%s%s%s", version, totalram, freeram, uptime, cputhreads);
	gtk_label_set_text(GTK_LABEL(label), displayText);

	// Frees variables when no longer needed
	free(version);
	free(totalram);
	free(freeram);
	free(uptime);
	free(cputhreads);
	g_free(displayText);
}
