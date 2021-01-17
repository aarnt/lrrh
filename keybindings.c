// BadWolf: Minimalist and privacy-oriented WebKitGTK+ browser
// Copyright © 2019-2020 Badwolf Authors <https://hacktivis.me/projects/badwolf>
// SPDX-License-Identifier: BSD-3-Clause

#include "keybindings.h"

#include "badwolf.h"

#include <glib/gi18n.h> /* _() */

static gboolean
about_dialogCb_activate_link(GtkAboutDialog *about_dialog, gchar *uri, gpointer user_data)
{
	(void)about_dialog;
	struct Window *window = (struct Window *)user_data;

	badwolf_new_tab(GTK_NOTEBOOK(window->notebook), new_browser(window, uri, NULL), FALSE);

	gtk_widget_destroy(GTK_WIDGET(about_dialog));

	return TRUE;
}

static void
badwolf_about_dialog(GtkWindow *main_window, gpointer user_data)
{
	struct Window *window   = (struct Window *)user_data;
	GtkWidget *about_dialog = gtk_about_dialog_new();

	char *comments = NULL;

	comments = g_strdup_printf(_("Minimalist and privacy-oriented WebKitGTK+ browser\n"
	                             "Runtime WebKit version: %d.%d.%d"),
	                           webkit_get_major_version(),
	                           webkit_get_minor_version(),
	                           webkit_get_micro_version());

	gtk_window_set_transient_for(GTK_WINDOW(about_dialog), main_window);
	gtk_window_set_destroy_with_parent(GTK_WINDOW(about_dialog), TRUE);

	gtk_about_dialog_set_license(GTK_ABOUT_DIALOG(about_dialog),
	                             "SPDX-License-Identifier: BSD-3-Clause");
	gtk_about_dialog_set_copyright(
	    GTK_ABOUT_DIALOG(about_dialog),
	    "2019-2020 Badwolf Authors <https://hacktivis.me/projects/badwolf>");
	gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(about_dialog), homepage);
	gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(about_dialog), comments);
	gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(about_dialog), version);
	gtk_about_dialog_set_logo_icon_name(GTK_ABOUT_DIALOG(about_dialog), "badwolf");

	g_signal_connect(about_dialog, "activate-link", G_CALLBACK(about_dialogCb_activate_link), window);

	(void)gtk_dialog_run(GTK_DIALOG(about_dialog));
	gtk_widget_destroy(about_dialog);
}

static void
toggle_caret_browsing(WebKitWebView *webView)
{
	WebKitSettings *settings = webkit_web_view_get_settings(webView);

	webkit_settings_set_enable_caret_browsing(settings,
	                                          !webkit_settings_get_enable_caret_browsing(settings));

	webkit_web_view_set_settings(webView, settings);
}

/*
 * Goto the next tab in notebook. If current is the last one, goto the second tab!
 */ 
static void
goto_next_tab(GtkNotebook *notebook)
{
	gint npages = gtk_notebook_get_n_pages(notebook);
	gint curr = gtk_notebook_get_current_page(notebook);
	if (curr+1 == npages)	
		gtk_notebook_set_current_page(notebook, 1);
	else gtk_notebook_next_page(notebook);
}

/* commonCb_key_press_event: Global callback for keybindings
 *
 * These shortcuts should be avoided as much as possible:
 * - Single key shortcuts (ie. backspace and space)
 * - Triple key shortcuts (except for Ctrl+Shift)
 * - Unix Terminal shortcuts (specially Ctrl-W)
 *
 * loosely follows https://developer.gnome.org/hig/stable/keyboard-input.html
 */
gboolean
commonCb_key_press_event(struct Window *window, GdkEvent *event, struct Client *browser)
{
	GtkNotebook *notebook = GTK_NOTEBOOK(window->notebook);
	struct Client *nbrowser = NULL;
	gdouble zoom = 0;

	if(((GdkEventKey *)event)->state & GDK_CONTROL_MASK)
	{
		if(browser != NULL)
		{
			switch(((GdkEventKey *)event)->keyval)
			{
			case GDK_KEY_F4: webkit_web_view_try_close(browser->webView); return TRUE;
			case GDK_KEY_r:
				if(((GdkEventKey *)event)->state & GDK_SHIFT_MASK)
					webkit_web_view_reload_bypass_cache(browser->webView);
				else
					webkit_web_view_reload(browser->webView);

				return TRUE;
			case GDK_KEY_f: gtk_widget_grab_focus(browser->search); return TRUE;
			case GDK_KEY_l: gtk_widget_grab_focus(browser->location); return TRUE;
			case GDK_KEY_0:
				webkit_web_view_set_zoom_level(WEBKIT_WEB_VIEW(browser->webView), 1);
				return TRUE;
			case GDK_KEY_p:
				webkit_print_operation_run_dialog(webkit_print_operation_new(browser->webView),
				                                  GTK_WINDOW(browser->window));
				return TRUE;
			case GDK_KEY_i:
				gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(browser->auto_load_images),
					!gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(browser->auto_load_images)));
				webkit_web_view_reload(browser->webView);
				return TRUE;
			case GDK_KEY_j:
				gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(browser->javascript),
					!gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(browser->javascript)));
				webkit_web_view_reload(browser->webView);
				return TRUE;
			case GDK_KEY_q:
				gtk_main_quit();
				return TRUE;
			case GDK_KEY_n:
				nbrowser = new_browser(window, 
				                       gtk_label_get_text(GTK_LABEL(browser->statuslabel)),
				                       NULL);
				if (nbrowser != NULL)
				{		
					badwolf_new_tab(GTK_NOTEBOOK(window->notebook), nbrowser, FALSE);
					gint npages = gtk_notebook_get_n_pages(GTK_NOTEBOOK(window->notebook));
					gtk_notebook_set_current_page(GTK_NOTEBOOK(window->notebook), npages-1);
				}	
				return TRUE;	
			case GDK_KEY_w:
				webkit_web_view_try_close(browser->webView);
				return TRUE;
			case GDK_KEY_plus:
				zoom = webkit_web_view_get_zoom_level(WEBKIT_WEB_VIEW(browser->webView));
				zoom += zoom * 0.1;
				webkit_web_view_set_zoom_level(WEBKIT_WEB_VIEW(browser->webView), zoom);
				return TRUE;
			case GDK_KEY_minus:
				zoom = webkit_web_view_get_zoom_level(WEBKIT_WEB_VIEW(browser->webView));
				zoom += zoom * -0.1;
				webkit_web_view_set_zoom_level(WEBKIT_WEB_VIEW(browser->webView), zoom);
				return TRUE;
			case GDK_KEY_Tab:
				goto_next_tab(notebook);		
				return TRUE;					
			}
		}
		else
		{
			switch(((GdkEventKey *)event)->keyval)
			{
			case GDK_KEY_Page_Down: gtk_notebook_next_page(notebook); return TRUE;
			case GDK_KEY_Page_Up: gtk_notebook_prev_page(notebook); return TRUE;
			case GDK_KEY_t: badwolf_new_tab(notebook, new_browser(window, NULL, NULL), TRUE); return TRUE;

			case GDK_KEY_q:
				gtk_main_quit();
				return TRUE;

			case GDK_KEY_Tab:
				/*npages = gtk_notebook_get_n_pages(notebook);
				curr = gtk_notebook_get_current_page(notebook);
				if (curr+1 == npages)	
					gtk_notebook_set_current_page(notebook, 1);
				else gtk_notebook_next_page(notebook);*/
				goto_next_tab(notebook);		
				return TRUE;
			}
		}
	}

	if((((GdkEventKey *)event)->state & GDK_MOD1_MASK))
	{
		if(browser != NULL)
		{
			switch(((GdkEventKey *)event)->keyval)
			{
				case GDK_KEY_Left: 
					webkit_web_view_go_back(browser->webView);
					return TRUE;
				case GDK_KEY_Right: 
					webkit_web_view_go_forward(browser->webView); 
					return TRUE;
			}
		}
		
		if((((GdkEventKey *)event)->keyval >= GDK_KEY_0) &&
		   (((GdkEventKey *)event)->keyval <= GDK_KEY_9))
			gtk_notebook_set_current_page(notebook, (gint)(((GdkEventKey *)event)->keyval - GDK_KEY_1));
	}

	if(browser != NULL)
	{
		switch(((GdkEventKey *)event)->keyval)
		{
		case GDK_KEY_F5: webkit_web_view_reload(browser->webView); return TRUE;
		case GDK_KEY_Escape: webkit_web_view_stop_loading(browser->webView); return TRUE;
		case GDK_KEY_F7: toggle_caret_browsing(browser->webView); return TRUE;
		case GDK_KEY_F12:
			webkit_web_inspector_show(webkit_web_view_get_inspector(browser->webView));
			return TRUE;
		}
	}
	else
	{
		switch(((GdkEventKey *)event)->keyval)
		{
		case GDK_KEY_F1: badwolf_about_dialog(GTK_WINDOW(window->main_window), window); return TRUE;
		}
	}

	return FALSE;
}

gboolean
WebViewCb_key_press_event(WebKitWebView *webView, GdkEvent *event, gpointer user_data)
{
	(void)webView;
	struct Client *browser = (struct Client *)user_data;

	if(commonCb_key_press_event(browser->window, event, browser)) return TRUE;

	return FALSE;
}

gboolean
boxCb_key_press_event(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
	(void)widget;
	struct Client *browser = (struct Client *)user_data;

	if(commonCb_key_press_event(browser->window, event, browser)) return TRUE;

	return FALSE;
}

gboolean
main_windowCb_key_press_event(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
	(void)widget;
	struct Window *window = (struct Window *)user_data;

	if(commonCb_key_press_event(window, event, NULL)) return TRUE;

	return FALSE;
}