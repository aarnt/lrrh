#include "bookmarks.h"

#include "badwolf.h"
#include "config.h"

#include <glib/gi18n.h>   /* _() and other internationalization/localization helpers */
#include <glib/gprintf.h> /* g_fprintf() */
#include <gtk/gtk.h>
#include <libxml/xinclude.h>
#include <libxml/xpath.h>
#include <unistd.h> /* access() */

static gboolean
location_completion_matches(GtkEntryCompletion *completion,
                            const gchar *key,
                            GtkTreeIter *iter,
                            gpointer user_data)
{
	(void)user_data;
	gchar *buffer;
	gchar *pattern;
	gboolean result;

	pattern = g_strdup_printf("*%s*", key);

	gtk_tree_model_get(gtk_entry_completion_get_model(completion), iter, 0, &buffer, -1);
	result = g_pattern_match_simple(pattern, buffer);

	g_free(buffer);
	g_free(pattern);
	return result;
}

void
bookmarks_completion_setup(GtkEntryCompletion *location_completion, GtkTreeModel *tree_model)
{
	gtk_entry_completion_set_model(location_completion, tree_model);
	gtk_entry_completion_set_text_column(location_completion, 0);
	gtk_entry_completion_set_match_func(location_completion, location_completion_matches, NULL, NULL);
	gtk_entry_completion_set_inline_selection(location_completion, BADWOLF_LOCATION_INLINE_SELECTION);
}

static void
location_completion_cleanup(xmlXPathObjectPtr xpathObj, xmlXPathContextPtr xpathCtx, xmlDocPtr doc)
{
	if(xpathObj != NULL) xmlXPathFreeObject(xpathObj);
	if(xpathCtx != NULL) xmlXPathFreeContext(xpathCtx);
	if(doc != NULL) xmlFreeDoc(doc);
}

static void
load_xpath_results(GtkListStore *list_store, xmlNodeSetPtr nodes)
{
	GtkTreeIter iter;
	int size;

	size = (nodes) ? nodes->nodeNr : 0;

	g_fprintf(stderr, _("Bookmarks: Found %d bookmarks.\n"), size);
	for(int i = 0; i < size; i++)
		if(nodes->nodeTab[i])
		{
			gtk_list_store_append(list_store, &iter);
			gtk_list_store_set(
			    list_store, &iter, 0, (char *)xmlXPathCastNodeToString(nodes->nodeTab[i]), -1);
		}
}

GtkTreeModel *
bookmarks_completion_init()
{
	const xmlChar *xpathExpr = (const xmlChar *)"//bookmark/@href";
  char *filename = g_build_filename(g_get_user_data_dir(), "lrrh", "bookmarks.xbel", NULL);
	xmlDocPtr doc  = NULL;
	xmlXPathContextPtr xpathCtx = NULL;
	xmlXPathObjectPtr xpathObj  = NULL;
	GtkListStore *list_store    = gtk_list_store_new(1, G_TYPE_STRING);

	if(access(filename, R_OK) != 0)
	{
		g_fprintf(stderr, _("Bookmarks: No loadable file found at %s\n"), filename);
		return NULL;
	}

	g_fprintf(stderr, _("Bookmarks: loading at %s\n"), filename);
	doc = xmlParseFile(filename);
	if(doc == NULL)
	{
		g_fprintf(stderr, _("Bookmarks: unable to parse file \"%s\"\n"), filename);
		location_completion_cleanup(xpathObj, xpathCtx, doc);
		return NULL;
	}

	xmlXIncludeProcess(doc);

	xpathCtx = xmlXPathNewContext(doc);
	if(xpathCtx == NULL)
	{
		g_fprintf(stderr, _("Bookmarks: unable to create new XPath context\n"));
		location_completion_cleanup(xpathObj, xpathCtx, doc);
		return NULL;
	}

	xpathObj = xmlXPathEvalExpression(xpathExpr, xpathCtx);
	if(xpathObj == NULL)
	{
		g_fprintf(stderr, _("Bookmarks: unable to evaluate XPath expression \"%s\"\n"), xpathExpr);
		location_completion_cleanup(xpathObj, xpathCtx, doc);
		return NULL;
	}

	load_xpath_results(list_store, xpathObj->nodesetval);

	location_completion_cleanup(xpathObj, xpathCtx, doc);

	g_fprintf(stderr, _("Bookmarks: Done.\n"));

	return GTK_TREE_MODEL(list_store);
}
