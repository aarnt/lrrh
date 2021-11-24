#ifndef TABSTACK_H_INCLUDED
#define TABSTACK_H_INCLUDED

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "badwolf.h"

struct tabstack
{
  GtkWidget *data;
  struct tabstack *prev;
  struct tabstack *next;
  //gboolean isRemoved;
};

//void display();
void tabstack_push(GtkWidget *);
gint tabstack_pop();
void tabstack_remove(GtkWidget *);

void
notebookPage_removed(GtkNotebook* self, GtkWidget* child, guint page_num, gpointer user_data);

#endif /* TABSTACK_H_INCLUDED */
