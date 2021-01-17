#ifndef BADWOLF_H_INCLUDED
#define BADWOLF_H_INCLUDED

#include <gtk/gtk.h>
#include <webkit2/webkit2.h>

extern const gchar *homepage;
extern const gchar *version;

struct Window
{
	GtkWidget *main_window;
	GtkWidget *notebook;
	GtkWidget *new_tab;
	GtkWidget *downloads_tab;
};

struct Client
{
	GtkWidget *box;

	GtkWidget *toolbar;
	GtkWidget *back;
	GtkWidget *forward;
	GtkWidget *javascript;
	GtkWidget *auto_load_images;
	GtkWidget *location;

	WebKitWebView *webView;
	struct Window *window;

	GtkWidget *statusbar;
	GtkWidget *statuslabel;
	GtkWidget *search;
};

GtkWidget *badwolf_new_tab_box(const gchar *title, struct Client *browser);
void webView_tab_label_change(struct Client *browser, const gchar *title);
struct Client *
new_browser(struct Window *window, const gchar *target_url, WebKitWebView *related_web_view);
int badwolf_new_tab(GtkNotebook *notebook, struct Client *browser, bool auto_switch);
gint badwolf_get_tab_position(GtkContainer *notebook, GtkWidget *child);
#endif /* BADWOLF_H_INCLUDED */
