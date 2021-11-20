#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED
/* BADWOLF_TAB_POSITION: Position of the tab listing, can be one of:
 * - GTK_POS_TOP
 * - GTK_POS_BOTTOM
 * - GTK_POS_RIGHT
 * - GTK_POS_LEFT
 *
 * See https://developer.gnome.org/gtk3/stable/gtk3-Standard-Enumerations.html#GtkPositionType
 */
#define BADWOLF_TAB_POSITION GTK_POS_TOP

/* BADWOLF_TAB_LABEL_CHARWIDTH: Amount of characters the tab label text fits
 * Quite conflicts with BADWOLF_TAB_BOX_WIDTH, recommended to only define one
 */
#define BADWOLF_TAB_LABEL_CHARWIDTH 26

/* BADWOLF_TAB_BOX_WIDTH: Requested width (in pixels) for the whole tab
 * Quite conflicts with BADWOLF_TAB_LABEL_CHARWIDTH, recommended to only define one
 */
//#define BADWOLF_TAB_BOX_WIDTH 120

// BADWOLF_TAB_HEXPAND: Should the tab try to fill the available horizontal space?
#define BADWOLF_TAB_HEXPAND FALSE

/* BADWOLF_TAB_LABEL_ELLIPSIZE: pango ellipsize mode of the tab label text, can be one of:
 * - PANGO_ELLIPSIZE_NONE
 * - PANGO_ELLIPSIZE_START
 * - PANGO_ELLIPSIZE_MIDDLE
 * - PANGO_ELLIPSIZE_END
 *
 * See https://developer.gnome.org/pango/stable/pango-Layout-Objects.html#PangoEllipsizeMode
 */
#define BADWOLF_TAB_LABEL_ELLIPSIZE PANGO_ELLIPSIZE_MIDDLE

// BADWOLF_BOX_PADDING: Amount of padding between browser’s box (tab child) elements
#define BADWOLF_BOX_PADDING 0

// BADWOLF_TOOLBAR_PADDING: Amount of padding between toolbar elements
#define BADWOLF_TOOLBAR_PADDING 0

// BADWOLF_TOOLBAR_PADDING: Amount of padding between toolbar elements
#define BADWOLF_TOOLBAR_SEPARATOR_PADDING 4

// BADWOLF_STATUSBAR_PADDING: Amount of padding between statusbar elements
#define BADWOLF_STATUSBAR_PADDING 0

// BADWOLF_DOWNLOAD_PADDING: Amount of padding between download list row-elements
#define BADWOLF_DOWNLOAD_PADDING 5

/* BADWOLF_DEFAULT_WIDTH / BADWOLF_DEFAULT_HEIGHT:
 * Used to define the default width/height of the window,
 * useful for floating Window Managers, probably useless in tiling ones
 *
 * See https://developer.gnome.org/gtk3/stable/GtkWindow.html#gtk-window-set-default-size
 */
#define BADWOLF_DEFAULT_WIDTH 800
#define BADWOLF_DEFAULT_HEIGHT 600

/* BADWOLF_WEBKIT_SETTINGS:
 * Used when creating a new view with webkit_settings_new_with_settings the usage is:
 * setting-name, setting-value, setting-name, …, NULL
 *
 * See: https://webkitgtk.org/reference/webkit2gtk/stable/WebKitSettings.html
 */
// clang-format off
#define BADWOLF_WEBKIT_SETTINGS \
	"default-charset", "utf-8", \
	"enable-accelerated-2d-canvas", FALSE, \
	"enable-caret-browsing", FALSE, \
	"enable-developer-extras", TRUE, \
	"enable-dns-prefetching", FALSE, \
	"enable-hyperlink-auditing", FALSE, \
	"enable-java", FALSE, \
	"enable-javascript-markup", FALSE, \
	"enable-javascript", TRUE, \
	"enable-plugins", FALSE, \
	"javascript-can-access-clipboard", FALSE, \
	"javascript-can-open-windows-automatically", FALSE, \
	"media-playback-requires-user-gesture", TRUE, \
	"minimum-font-size", 9, \
	"allow-top-navigation-to-data-urls", FALSE, \
	NULL
// clang-format on

/* BADWOLF_STATUSLABEL_ELLIPSIZE: pango ellipsize mode of the status bar label text, can be one of:
 * - PANGO_ELLIPSIZE_NONE
 * - PANGO_ELLIPSIZE_START
 * - PANGO_ELLIPSIZE_MIDDLE
 * - PANGO_ELLIPSIZE_END
 *
 * See https://developer.gnome.org/pango/stable/pango-Layout-Objects.html#PangoEllipsizeMode
 */
#define BADWOLF_STATUSLABEL_ELLIPSIZE PANGO_ELLIPSIZE_MIDDLE

/* BADWOLF_DOWNLOAD_FILE_PATH_ELLIPSIZE: pango ellipsize mode of the download destination path,
 * can be one of:
 * - PANGO_ELLIPSIZE_NONE
 * - PANGO_ELLIPSIZE_START
 * - PANGO_ELLIPSIZE_MIDDLE
 * - PANGO_ELLIPSIZE_END
 *
 * See https://developer.gnome.org/pango/stable/pango-Layout-Objects.html#PangoEllipsizeMode
 */
#define BADWOLF_DOWNLOAD_FILE_PATH_ELLIPSIZE PANGO_ELLIPSIZE_MIDDLE

// BADWOLF_LOCATION_INLINE_SELECTION: show selected completion as a selection in location entry
#define BADWOLF_LOCATION_INLINE_SELECTION TRUE

#endif /* CONFIG_H_INCLUDED */
