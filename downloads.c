// BadWolf: Minimalist and privacy-oriented WebKitGTK+ browser
// Copyright © 2019 Haelwenn (lanodan) Monnier <contact@hacktivis.me>
// SPDX-License-Identifier: BSD-3-Clause

#include "downloads.h"

#include "badwolf.h"
#include "config.h"

#include <glib/gi18n.h> /* _() and other internationalization/localization helpers */

static void
download_stop_iconCb_clicked(GtkButton *stop_icon, gpointer user_data)
{
	(void)stop_icon;
	WebKitDownload *webkit_download = (WebKitDownload *)user_data;

	webkit_download_cancel(webkit_download);
}

void
download_format_elapsed(char *formatted, size_t formatted_size, char *format, int total)
{
	snprintf(formatted,
	         formatted_size,
	         format,
	         total / 3600,        /* hours */
	         (total % 3600) / 60, /* minutes */
	         total % 60);         /* seconds */
}

void
download_new_entry(WebKitDownload *webkit_download, struct Download *download)
{
	download->error     = 0;
	download->container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, BADWOLF_DOWNLOAD_PADDING);
	download->progress  = gtk_progress_bar_new();
	download->file_path = gtk_label_new(NULL);
	download->status    = gtk_label_new(_("Download starting…"));
	download->icon =
	    gtk_image_new_from_icon_name("network-idle-symbolic", GTK_ICON_SIZE_SMALL_TOOLBAR);
	download->stop_icon = gtk_button_new_from_icon_name("process-stop", GTK_ICON_SIZE_SMALL_TOOLBAR);

	gtk_progress_bar_set_show_text(GTK_PROGRESS_BAR(download->progress), TRUE);
	gtk_label_set_ellipsize(GTK_LABEL(download->file_path), BADWOLF_DOWNLOAD_FILE_PATH_ELLIPSIZE);

	g_signal_connect(
	    download->stop_icon, "clicked", G_CALLBACK(download_stop_iconCb_clicked), webkit_download);

	gtk_box_pack_start(GTK_BOX(download->container), download->icon, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(download->container), download->progress, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(download->container), download->stop_icon, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(download->container), download->status, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(download->container), download->file_path, FALSE, FALSE, 0);

	gtk_list_box_insert(GTK_LIST_BOX(download->window->downloads_tab), download->container, -1);

	gtk_widget_show_all(download->container);
}

void
downloadCb_created_destination(WebKitDownload *webkit_download,
                               gchar *destination,
                               gpointer user_data)
{
	(void)webkit_download;
	char *markup;
	struct Download *download = (struct Download *)user_data;

	markup = g_markup_printf_escaped(
	    "<a href=\"%s\">%s</a>", destination, webkit_uri_for_display(destination));

	gtk_label_set_markup(GTK_LABEL(download->file_path), markup);
	g_free(markup);
}

gboolean
downloadCb_decide_destination(WebKitDownload *webkit_download,
                              gchar *suggested_filename,
                              gpointer user_data)
{
	struct Client *browser = (struct Client *)user_data;
	gint chooser_response;
	GtkWindow *parent_window = GTK_WINDOW(browser->window->main_window);

	GtkFileChooserNative *file_dialog =
	    gtk_file_chooser_native_new(NULL, parent_window, GTK_FILE_CHOOSER_ACTION_SAVE, NULL, NULL);
	GtkFileChooser *file_chooser = GTK_FILE_CHOOSER(file_dialog);

	gtk_file_chooser_set_current_name(file_chooser, suggested_filename);
	gtk_file_chooser_set_do_overwrite_confirmation(file_chooser, TRUE);
	webkit_download_set_allow_overwrite(webkit_download, TRUE);

	chooser_response = gtk_native_dialog_run(GTK_NATIVE_DIALOG(file_dialog));

	if(chooser_response == GTK_RESPONSE_ACCEPT)
		webkit_download_set_destination(webkit_download, gtk_file_chooser_get_uri(file_chooser));
	else
		webkit_download_cancel(webkit_download);

	g_object_unref(file_dialog);

	return FALSE; /* Let it propagate */
}

void
downloadCb_failed(WebKitDownload *webkit_download, GError *error, gpointer user_data)
{
	struct Download *download = (struct Download *)user_data;
	char formatted[BUFSIZ];
	int total = (int)webkit_download_get_elapsed_time(webkit_download);
	char *format;

	download->error = error;

	if(g_error_matches(error, WEBKIT_DOWNLOAD_ERROR, WEBKIT_DOWNLOAD_ERROR_CANCELLED_BY_USER))
		format = _("%02i:%02i:%02i Download cancelled");
	else
		format = _("%02i:%02i:%02i Download error");

	download_format_elapsed(formatted, sizeof(formatted), format, total);

	gtk_label_set_text(GTK_LABEL(download->status), formatted);

	gtk_widget_destroy(download->stop_icon);

	gtk_image_set_from_icon_name(
	    GTK_IMAGE(download->icon), "network-error-symbolic", GTK_ICON_SIZE_SMALL_TOOLBAR);
}

void
downloadCb_finished(WebKitDownload *webkit_download, gpointer user_data)
{
	struct Download *download = (struct Download *)user_data;
	char formatted[BUFSIZ];
	int total = (int)webkit_download_get_elapsed_time(webkit_download);

	gchar *format_size = g_format_size(webkit_download_get_received_data_length(webkit_download));

	download_format_elapsed(
	    formatted, sizeof(formatted), _("%02i:%02i:%02i Download finished"), total);

	gtk_widget_destroy(download->stop_icon);

	if(download->error == 0)
	{
		gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(download->progress), 1);
		gtk_progress_bar_set_text(GTK_PROGRESS_BAR(download->progress), format_size);
		gtk_label_set_text(GTK_LABEL(download->status), formatted);
		gtk_image_set_from_icon_name(
		    GTK_IMAGE(download->icon), "network-idle-symbolic", GTK_ICON_SIZE_SMALL_TOOLBAR);
	}

	// TODO: Send notification
}

void
downloadCb_received_data(WebKitDownload *webkit_download, guint64 data_lenght, gpointer user_data)
{
	(void)data_lenght;
	struct Download *download = (struct Download *)user_data;
	char formatted[BUFSIZ];
	int total = (int)webkit_download_get_elapsed_time(webkit_download);

	gchar *format_size = g_format_size(webkit_download_get_received_data_length(webkit_download));

	download_format_elapsed(formatted, sizeof(formatted), _("%02i:%02i:%02i Downloading…"), total);

	gtk_image_set_from_icon_name(
	    GTK_IMAGE(download->icon), "network-receive-symbolic", GTK_ICON_SIZE_SMALL_TOOLBAR);
	gtk_label_set_text(GTK_LABEL(download->status), formatted);
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(download->progress),
	                              webkit_download_get_estimated_progress(webkit_download));
	gtk_progress_bar_set_text(GTK_PROGRESS_BAR(download->progress), format_size);

	g_free(format_size);
}

GtkWidget *
badwolf_downloads_tab_new()
{
	return gtk_list_box_new();
}

void
badwolf_downloads_tab_attach(struct Window *window)
{
	GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
	gtk_widget_set_name(scrolled_window, "browser__scrollwin_downloads");
	gtk_container_add(GTK_CONTAINER(scrolled_window), window->downloads_tab);
	gtk_notebook_insert_page(GTK_NOTEBOOK(window->notebook), scrolled_window, NULL, 0);

	gtk_notebook_set_tab_reorderable(GTK_NOTEBOOK(window->notebook), scrolled_window, TRUE);

	GtkWidget *tab_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_widget_set_name(tab_box, "browser__tabbox");
	GtkWidget *icon = gtk_image_new_from_icon_name("emblem-downloads", GTK_ICON_SIZE_SMALL_TOOLBAR);
	gtk_widget_set_name(icon, "browser__tabbox__icon");
	GtkWidget *label = gtk_label_new(_("Downloads"));
	gtk_widget_set_name(label, "browser__tabbox__label");

#ifdef BADWOLF_TAB_BOX_WIDTH
	gtk_widget_set_size_request(label, BADWOLF_TAB_BOX_WIDTH, -1);
#endif
#ifdef BADWOLF_TAB_LABEL_CHARWIDTH
	gtk_label_set_width_chars(GTK_LABEL(label), BADWOLF_TAB_LABEL_CHARWIDTH);
#endif
	gtk_widget_set_hexpand(tab_box, BADWOLF_TAB_HEXPAND);

	gtk_label_set_ellipsize(GTK_LABEL(label), BADWOLF_TAB_LABEL_ELLIPSIZE);
	gtk_label_set_single_line_mode(GTK_LABEL(label), TRUE);

	gtk_box_pack_start(GTK_BOX(tab_box), icon, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(tab_box), label, TRUE, TRUE, 0);

	gtk_widget_set_tooltip_text(tab_box, _("LRRH Downloads"));
	gtk_notebook_set_tab_label(GTK_NOTEBOOK(window->notebook), scrolled_window, tab_box);
	gtk_notebook_set_menu_label_text(
	    GTK_NOTEBOOK(window->notebook), scrolled_window, _("LRRH Downloads"));

	gtk_widget_show_all(tab_box);
}
