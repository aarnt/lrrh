#include <gtk/gtk.h>
GtkTreeModel *bookmarks_completion_init();
void bookmarks_completion_setup(GtkEntryCompletion *location_completion, GtkTreeModel *tree_model);
