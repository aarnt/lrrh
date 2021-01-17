#include "badwolf.h"

#include <gtk/gtk.h>

struct Download
{
	struct Window *window;

	GtkWidget *container;
	GtkWidget *icon;
	GtkWidget *stop_icon;
	GtkWidget *file_path;
	GtkWidget *progress;
	GtkWidget *status;
	GError *error;
};

void download_new_entry(WebKitDownload *webkit_download, struct Download *download);
void
downloadCb_created_destination(WebKitDownload *download, gchar *destination, gpointer user_data);
gboolean downloadCb_decide_destination(WebKitDownload *download,
                                       gchar *suggested_filename,
                                       gpointer user_data);
void downloadCb_failed(WebKitDownload *webkit_download, GError *error, gpointer user_data);
void downloadCb_finished(WebKitDownload *download, gpointer user_data);
void downloadCb_received_data(WebKitDownload *download, guint64 data_lenght, gpointer user_data);
GtkWidget *badwolf_downloads_tab_new();
void badwolf_downloads_tab_attach(struct Window *window);
