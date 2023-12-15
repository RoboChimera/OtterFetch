#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>

#include "fetch.h"

void setImage(GtkWidget *image, GtkWidget *grid) {
	const char *homeDir = getenv("HOME");

	if (homeDir != NULL) {
		int dirCSize = snprintf(NULL, 0, "%s/.config/otterfetch/banner.png", homeDir) + 1;
		char *dir = (char *)malloc(dirCSize);
		sprintf(dir, "%s/.config/otterfetch/banner.png", homeDir);

		image = gtk_image_new_from_file(dir);
		gtk_grid_attach(GTK_GRID(grid), image, 0, 0, 1, 1);
	} else {
		printf("No home directory found");
		return;
	}
}

int main(int argc, char *argv[]) {
	GtkWidget *window, *grid, *label, *watermark, *image;
	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "OtterFetch");
	gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

	// Making the grid
	grid = gtk_grid_new();
	gtk_container_add(GTK_CONTAINER(window), grid);
	
	// Image
	image = gtk_image_new_from_file("");
	setImage(image, grid);

	// Label
	label = gtk_label_new(NULL);
	fetch(label);
	gtk_widget_set_halign(GTK_WIDGET(label), GTK_ALIGN_START);

	watermark = gtk_label_new("Made by RoboChimera, A True Otter(TM)");
	gtk_widget_set_halign(GTK_WIDGET(watermark), GTK_ALIGN_END);

	// Attaching the GtkWidgets
	gtk_grid_attach(GTK_GRID(grid), image, 0, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), label, 0, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), watermark, 0, 2, 1, 1);

	gtk_widget_show_all(window);
	gtk_main();

	return 0;
}
