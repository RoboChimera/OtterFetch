/*
 * function		description		function name		library directory/file
 *
 * OS + Version		uname -sr		fetchUname()		components/fetchUname.h
 *
 * totalram		total memory in GB	fetchTotalram()		components/fetchTotalRam.h
 * freeram		free memory in GB	fetchFreeram()		components/fetchUname.h
*/

// Include only variables we need
#include "components/fetchTotalRam.h"
#include "components/fetchFreeRam.h"
#include "components/fetchUname.h"

void fetch(GtkWidget *label) {
	char *totalram = fetchTotalram();
	char *freeram = fetchFreeram();
	char *version = fetchUname();

	// Configurates the GTK label
	char *displayText = g_strdup_printf("%s%s%s", version, totalram, freeram);
	gtk_label_set_text(GTK_LABEL(label), displayText);

	// Frees variables when no longer needed
	free(version);
	free(totalram);
	free(freeram);
	g_free(displayText);
}
