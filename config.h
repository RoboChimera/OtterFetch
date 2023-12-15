/*
 * function		description		function name
 *
 * OS + Version		uname -sr		fetchUname()
 *
 * totalram		total memory in GB	fetchTotalram()
 * freeram		free memory in GB	fetchFreeram()
*/

#include "components/fetchTotalRam.h"
#include "components/fetchFreeRam.h"
#include "components/fetchUname.h"

void fetch(GtkWidget *label) {
	// Functions we want to call
	char *totalram = fetchTotalram();
	char *freeram = fetchFreeram();
	char *version = fetchUname();

	// Configurate the GTK label
	char *displayText = g_strdup_printf("%s%s%s", version, totalram, freeram);
	gtk_label_set_text(GTK_LABEL(label), displayText);

	// Free variables when no longer needed
	free(version);
	free(totalram);
	free(freeram);
	g_free(displayText);
}
