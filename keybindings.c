// BadWolf: Minimalist and privacy-oriented WebKitGTK+ browser
// Copyright © 2019-2021 Badwolf Authors <https://hacktivis.me/projects/badwolf>
// SPDX-License-Identifier: BSD-3-Clause

#include "keybindings.h"
#include "badwolf.h"
#include <glib/gi18n.h> /* _() */

static void open_site_on_new_tab(struct Window *window, const gchar *url, gboolean jsEnabled);
static void refreshWebView(WebKitWebView* webView);
static void goto_prev_tab(GtkNotebook *notebook);
static void goto_next_tab(GtkNotebook *notebook);
static void web_view_get_selected_text(WebKitWebView *web_view, struct Window *window);

static gboolean
about_dialogCb_activate_link(GtkAboutDialog *about_dialog, gchar *uri, gpointer user_data)
{
  (void)about_dialog;
  struct Window *window = (struct Window *)user_data;

  badwolf_new_tab(GTK_NOTEBOOK(window->notebook), new_browser(window, uri, NULL), TRUE, FALSE);
  gtk_window_close(GTK_WINDOW(about_dialog));

  return TRUE;
}

static void
badwolf_about_dialog(GtkWindow *main_window, gpointer user_data)
{
  struct Window *window   = (struct Window *)user_data;
  GtkWidget *about_dialog = gtk_about_dialog_new();

  char *comments = NULL;

  comments = g_strdup_printf(_("Another tiny & fast & functional WebKitGTK browser\n"
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
    "2019-2021 Badwolf Authors <https://hacktivis.me/projects/badwolf>\n2021-2023 Alexandre A Arnt <https://tintaescura.com>");
  gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(about_dialog), homepage);
  gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(about_dialog), comments);
  gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(about_dialog), version);
  gtk_about_dialog_set_logo_icon_name(GTK_ABOUT_DIALOG(about_dialog), "lrrh");

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
	const gchar *duckUrl = "https://lite.duckduckgo.com";
	gchar *statusLabel = NULL;

  if ((((GdkEventKey *)event)->state & (GDK_CONTROL_MASK | GDK_SHIFT_MASK)) == (GDK_CONTROL_MASK | GDK_SHIFT_MASK) && !isKioskMode())
 	{
    if (browser != NULL)
		{
      gboolean jsEnabled = gtk_toggle_button_get_active((GtkToggleButton *)browser->javascript);
      gboolean imgEnabled = gtk_toggle_button_get_active((GtkToggleButton *)browser->auto_load_images);

      switch(((GdkEventKey *)event)->keyval)
			{
      case GDK_KEY_C:
        statusLabel = strdup(gtk_label_get_text(GTK_LABEL(browser->statuslabel)));
        if (statusLabel != NULL)
        {
          /* Set clipboard text */
          gtk_clipboard_set_text(gtk_widget_get_clipboard(GTK_WIDGET(browser->webView), GDK_SELECTION_CLIPBOARD), statusLabel, -1);
        }
        return TRUE;
      case GDK_KEY_ISO_Left_Tab:
				goto_prev_tab(notebook);
				return TRUE;
      case GDK_KEY_N:
        nbrowser = new_browser(window,
                               gtk_label_get_text(GTK_LABEL(browser->statuslabel)),
                               NULL);
        if (nbrowser != NULL)
        {
          gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(nbrowser->javascript), jsEnabled);
          gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(nbrowser->auto_load_images), imgEnabled);
          badwolf_new_tab(GTK_NOTEBOOK(window->notebook), nbrowser, FALSE, FALSE);
          gtk_notebook_set_current_page(GTK_NOTEBOOK(window->notebook),
          gtk_notebook_get_current_page(notebook)+1);
        }
        return TRUE;
      case GDK_KEY_r:
        webkit_web_view_reload_bypass_cache(browser->webView);
        return TRUE;
      }
		}
	}
  else if(((GdkEventKey *)event)->state & GDK_CONTROL_MASK && !isKioskMode())
	{
    if(browser != NULL)
		{
			switch(((GdkEventKey *)event)->keyval)
			{
			case GDK_KEY_0:
				webkit_web_view_set_zoom_level(WEBKIT_WEB_VIEW(browser->webView), 1);
				return TRUE;
      case GDK_KEY_equal:
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
			case GDK_KEY_d:
				open_site_on_new_tab(window, duckUrl, false);
				return TRUE;
			case GDK_KEY_f:
				gtk_widget_grab_focus(browser->search);
				return TRUE;
			case GDK_KEY_h:
				gtk_widget_grab_focus(GTK_WIDGET (browser->webView));
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
			case GDK_KEY_l:
				gtk_widget_grab_focus(browser->location);
				return TRUE;
			case GDK_KEY_p:
				webkit_print_operation_run_dialog(webkit_print_operation_new(browser->webView),
				                                  GTK_WINDOW(browser->window));
				return TRUE;
			case GDK_KEY_q:
        gtk_main_quit(); //webkit_web_view_try_close(browser->webView);
				return TRUE;
			case GDK_KEY_r:
        webkit_web_view_reload(browser->webView);
				return TRUE;
			case GDK_KEY_w:
        webkit_web_view_try_close(browser->webView);
				return TRUE;
			case GDK_KEY_z:
				web_view_get_selected_text(WEBKIT_WEB_VIEW(browser->webView), window);
				return TRUE;
			}
		}
    else
		{
			switch(((GdkEventKey *)event)->keyval)
			{
			case GDK_KEY_Page_Down:
				gtk_notebook_next_page(notebook);
				return TRUE;
			case GDK_KEY_Page_Up:
				gtk_notebook_prev_page(notebook);
				return TRUE;
			case GDK_KEY_Tab:
				goto_next_tab(notebook);
				return TRUE;
			case GDK_KEY_d:
				open_site_on_new_tab(window, duckUrl, false);
				return TRUE;
			case GDK_KEY_q:
        gtk_main_quit();
				return TRUE;
			case GDK_KEY_t:
        badwolf_new_tab(notebook, new_browser(window, NULL, NULL), TRUE, FALSE);
				return TRUE;
			}
		}
	}
  else if(((GdkEventKey *)event)->state & GDK_MOD1_MASK && !isKioskMode())
	{
    if(browser != NULL && !isKioskMode(browser))
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

  if (browser != NULL && !isKioskMode())
	{
		switch(((GdkEventKey *)event)->keyval)
		{
		case GDK_KEY_Escape: webkit_web_view_stop_loading(browser->webView); return TRUE;
    case GDK_KEY_F4: toggle_dark_mode(browser->webView); return TRUE;
    case GDK_KEY_F5: refreshWebView(browser->webView); return TRUE;
		case GDK_KEY_F7: toggle_caret_browsing(browser->webView); return TRUE;
    case GDK_KEY_F11: toggle_kiosk_mode(browser); return TRUE;
    case GDK_KEY_F12:
			webkit_web_inspector_show(webkit_web_view_get_inspector(browser->webView));
			return TRUE;
		}
  }
  else if (browser != NULL)
  {
    switch(((GdkEventKey *)event)->keyval)
    {
    case GDK_KEY_F11: toggle_kiosk_mode(browser); return TRUE;
    }
  }
  else if (!isKioskMode())
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

gboolean
tab_boxCb_button_release_event(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
  (void)widget;
  struct Client *browser = (struct Client *)user_data;

  if(((GdkEventButton *)event)->button == GDK_BUTTON_MIDDLE)
  {
    webkit_web_view_try_close(browser->webView);
    return TRUE;
  }
  return FALSE;
}

//-------------------------------- LRRH changes -------------------------------------//

/*
  Whenever user presses F5 key...
*/
static void
refreshWebView(WebKitWebView* webView)
{
  webkit_web_view_reload(webView);
  set_dark_mode(webView);
}

/*
 * Opens given url on new tab and focus webview widget
 */
static void
open_site_on_new_tab(struct Window *window, const gchar *url, gboolean jsEnabled)
{
  struct Client *nbrowser = NULL;
  nbrowser = new_browser(window, url, NULL);
  GtkNotebook *notebook = GTK_NOTEBOOK(window->notebook);

  if (nbrowser != NULL)
  {
    badwolf_new_tab(GTK_NOTEBOOK(window->notebook), nbrowser, FALSE, FALSE);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(nbrowser->javascript), jsEnabled);
    gtk_notebook_set_current_page(GTK_NOTEBOOK(window->notebook),
        gtk_notebook_get_current_page(notebook)+1);
  }

  gtk_widget_grab_focus(GTK_WIDGET(nbrowser->webView));
}

/*
 * Goto the previous tab in notebook. If current is the second one, goto the last tab!
 */
static void
goto_prev_tab(GtkNotebook *notebook)
{
  gint npages = gtk_notebook_get_n_pages(notebook);
  gint curr = gtk_notebook_get_current_page(notebook);
  if (curr-1 == 0)
    gtk_notebook_set_current_page(notebook, npages-1);
  else gtk_notebook_prev_page(notebook);
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

/*
 * Removes a substring sub from a given string str
 */
gchar *
strremove(gchar *str, const gchar *sub)
{
  size_t len = strlen(sub);
  if (len > 0) {
    char *p = str;
    while ((p = strstr(p, sub)) != NULL)
    {
      memmove(p, p + len, strlen(p + len) + 1);
    }
  }
  return str;
}

/*
 * Retrives given system LANGUAGE code, based on LC_TIME environment var
 */
gchar *
getLangCode()
{
  gchar *result = "";
  gchar *lctime = getenv("LC_TIME");
  if (lctime == NULL) return "";

  result = strremove(lctime, ".UTF-8");

  char *pos = strchr(result, '_');
  if(pos != NULL)
  {
    result = strremove(strdup(result), pos);
  }

  return result;
}

/*
  Opens the Google translate site to get a translated version of the user selected content
*/
#if WEBKIT_CHECK_VERSION(2, 40, 0)
static void
web_view_javascript_get_selected_text_finished(GObject  *object,
                             GAsyncResult *result,
                             gpointer      user_data)
{
  JSCValue               *value;
  GError                 *error = NULL;
  struct Window *window = (struct Window*)user_data;
  gchar *lang = NULL;

  value = webkit_web_view_evaluate_javascript_finish (WEBKIT_WEB_VIEW (object), result, &error);
  if (!value) {
    g_warning ("Error running javascript: %s", error->message);
	g_error_free (error);
	return;
  }

  if(jsc_value_is_undefined(value))
  {
    printf("Value is undefined!\n\n");
  }
  else if(jsc_value_is_string (value))
  {
    JSCException *exception;
    gchar        *str_value;

    str_value = jsc_value_to_string (value);
    exception = jsc_context_get_exception (jsc_value_get_context (value));

    if (!exception)
    {
      lang = getLangCode();

      gchar gtrans[64]="";
      gchar *gsite="https://translate.google.com/?sl=auto&tl=";
      strcat(gtrans, gsite);
      strcat(gtrans, lang);
      strcat(gtrans, "&q=");

      char **split = g_strsplit(str_value, " ", -1);
      g_free(str_value);
      str_value = g_strjoinv("+", split);
      g_strfreev(split);

      gchar *url=g_strconcat(gtrans, str_value, NULL);
      open_site_on_new_tab(window, url, true);
      g_free(url);
    }

    g_free (str_value);
  }
}
#else
static void
web_view_javascript_get_selected_text_finished(GObject  *object,
                             GAsyncResult *result,
                             gpointer      user_data)
{
	WebKitJavascriptResult *js_result;
	JSCValue               *value;
	GError                 *error = NULL;
  struct Window *window = (struct Window*)user_data;
  gchar *lang = NULL;

	js_result = webkit_web_view_run_javascript_finish (WEBKIT_WEB_VIEW (object), result, &error);
	if (!js_result) {
		g_warning ("Error running javascript: %s", error->message);
		g_error_free (error);
		return;
	}

	value = webkit_javascript_result_get_js_value (js_result);
	if(jsc_value_is_undefined(value))
  {
    printf("Value is undefined!\n\n");
  }
  else if(jsc_value_is_string (value))
  {
    JSCException *exception;
    gchar        *str_value;

    str_value = jsc_value_to_string (value);
    exception = jsc_context_get_exception (jsc_value_get_context (value));

    if (!exception)
    {
      lang = getLangCode();

      gchar gtrans[64]="";
      gchar *gsite="https://translate.google.com/?sl=auto&tl=";
      strcat(gtrans, gsite);
      strcat(gtrans, lang);
      strcat(gtrans, "&q=");

      char **split = g_strsplit(str_value, " ", -1);
      g_free(str_value);
      str_value = g_strjoinv("+", split);
      g_strfreev(split);

      gchar *url=g_strconcat(gtrans, str_value, NULL);
      open_site_on_new_tab(window, url, true);
      g_free(url);
    }

    g_free (str_value);

    webkit_javascript_result_unref (js_result);
  }
}
#endif

/*
  Calls a javascript script to execute some computation over the user selected text
*/
static void
web_view_get_selected_text(WebKitWebView *web_view, struct Window *window)
{
  const gchar *script = "window.getSelection().toString();";

#if WEBKIT_CHECK_VERSION(2, 40, 0)
  webkit_web_view_evaluate_javascript (WEBKIT_WEB_VIEW(web_view),
    script, -1, NULL, NULL, NULL, web_view_javascript_get_selected_text_finished, window);
#else
  webkit_web_view_run_javascript (WEBKIT_WEB_VIEW(web_view),
    script, NULL, web_view_javascript_get_selected_text_finished, window);
#endif    
}
