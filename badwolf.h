#ifndef BADWOLF_H_INCLUDED
#define BADWOLF_H_INCLUDED

#include <gtk/gtk.h>
#include <inttypes.h> /* uint64_t */
#include <webkit2/webkit2.h>

#if !WEBKIT_CHECK_VERSION(2, 32, 0)
#error WebkitGTK 2.32.0 is the latest supported version for badwolf.
#endif

extern const gchar *homepage;
extern const gchar *version;

struct Window
{
	GtkWidget *main_window;
	GtkWidget *notebook;
	GtkWidget *new_tab;
	GtkWidget *downloads_tab;
  WebKitUserContentManager *content_manager;
  WebKitUserContentFilterStore *content_store;
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

  uint64_t context_id;
  WebKitWebView *webView;
	struct Window *window;

	GtkWidget *statusbar;
	GtkWidget *statuslabel;
	GtkWidget *search;
};

GtkWidget *badwolf_new_tab_box(const gchar *title, struct Client *browser);

gboolean isKioskMode();
void toggle_kiosk_mode(struct Client *browser);
void set_kiosk_mode(struct Client *browser);
void toggle_dark_mode(WebKitWebView *web_view);
void set_dark_mode(WebKitWebView *web_view);
void webView_tab_label_change(struct Client *browser, const gchar *title);
struct Client *
new_browser(struct Window *window, const gchar *target_url, struct Client *old_browser);
int badwolf_new_tab(GtkNotebook *notebook, struct Client *browser, bool auto_switch, bool force_kiosk_mode);
gint badwolf_get_tab_position(GtkContainer *notebook, GtkWidget *child);
#endif /* BADWOLF_H_INCLUDED */
