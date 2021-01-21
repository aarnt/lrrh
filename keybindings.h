#ifndef KEYBINDINGS_H_INCLUDED
#define KEYBINDINGS_H_INCLUDED
#include "badwolf.h"

#include <gtk/gtk.h>
#include <webkit2/webkit2.h>

static void open_site_on_new_tab(struct Window *window, const gchar *url, gboolean jsEnabled);
gboolean boxCb_key_press_event(GtkWidget *widget, GdkEvent *event, gpointer user_data);
gboolean commonCb_key_press_event(struct Window *window, GdkEvent *event, struct Client *browser);
gboolean main_windowCb_key_press_event(GtkWidget *widget, GdkEvent *event, gpointer user_data);
gboolean WebViewCb_key_press_event(WebKitWebView *webView, GdkEvent *event, gpointer user_data);
#endif /* KEYBINDINGS_H_INCLUDED */
