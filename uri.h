#ifndef URI_H_INCLUDED
#define URI_H_INCLUDED
#include <glib.h>

/* badwolf_ensure_uri_scheme: tries to add a scheme on a pseudo-URL missing a scheme
 * - gchar text: pseudo-URL missing a scheme
 * - gboolean try_file: when TRUE check try first if it can be a file:// path
 *
 * When `text` isn't exploitable (ie. NULL), returns "about:blank",
 * when the URL seems to be valid, return it,
 * if try_file is TRUE, check if it can be file:// path,
 * some other checks might be added.
 * In the end use the fallback (`http://` for now, might get configuration),
 * might get some safeguard.
 */
const gchar *badwolf_ensure_uri_scheme(const gchar *text, gboolean try_file);
#endif /* URI_H_INCLUDED */
