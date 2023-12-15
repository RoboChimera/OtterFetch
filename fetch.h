#include "components/fetchTotalRam.h"
#include "components/fetchFreeRam.h"
#include "components/fetchUname.h"

void fetch(GtkWidget *label) {
	char *totalram = fetchTotalram();
	char *freeram = fetchFreeram();
	char *version = fetchUname();

	// Update the GTK label
	char *displayText = g_strdup_printf("%s%s%s", version, totalram, freeram);
	gtk_label_set_text(GTK_LABEL(label), displayText);

	free(version);
	free(totalram);
	free(freeram);
	g_free(displayText);
}
