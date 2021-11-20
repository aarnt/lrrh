#ifndef KEYBINDINGS_H_INCLUDED
#define KEYBINDINGS_H_INCLUDED
#include "badwolf.h"

#include <gtk/gtk.h>
#include <webkit2/webkit2.h>

GtkTreeModel *bookmarks_completion_init();
void bookmarks_completion_setup(GtkEntryCompletion *location_completion, GtkTreeModel *tree_model);
gboolean boxCb_key_press_event(GtkWidget *widget, GdkEvent *event, gpointer user_data);
gboolean commonCb_key_press_event(struct Window *window, GdkEvent *event, struct Client *browser);
gboolean main_windowCb_key_press_event(GtkWidget *widget, GdkEvent *event, gpointer user_data);
gboolean WebViewCb_key_press_event(WebKitWebView *webView, GdkEvent *event, gpointer user_data);
gboolean tab_boxCb_button_release_event(GtkWidget *widget, GdkEvent *event, gpointer user_data);
#endif /* KEYBINDINGS_H_INCLUDED */
