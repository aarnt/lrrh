#include "tabstack.h"

struct tabstack *prev = NULL;
struct tabstack *top  = NULL;

/*
 * This is the structure that controls the list of last visited tabs
 */

/*
 * Stacks the given tab widget visited by the user
 */
void tabstack_push(GtkWidget *item)
{
  struct tabstack *nptr = malloc(sizeof(struct tabstack));
  nptr->data = item;

  //printf("Pushed gtkWidget: 0x%" PRIXPTR "\n", (uintptr_t)nptr->data);
  nptr->prev = top;
  prev = top;
  nptr->next = top;
  top = nptr;
}

/*
 * Retrieves last tab index visited by the user
 */
gint tabstack_pop(GtkNotebook *notebook)
{
  /*struct node *temp;
  temp = top;
  top = top->next;
  printf("\n\n%d deleted", temp->data);
  free(temp);*/

  gint tabtogo = 0;
  prev = prev->prev;
  struct tabstack *temp = prev;

  if (temp == NULL)
  {
    return tabtogo;
  }
  else
  {
    while (temp != NULL)
    {
      if (temp->data == NULL)
      {
        prev = top;
        top = top->next;
        temp = top;
      }
      else break;
    }

    //printf("Planing to focus gtkWidget: 0x%" PRIXPTR "\n", (uintptr_t)temp->data);
    tabtogo = gtk_notebook_page_num(notebook, temp->data);
    //printf("TAB TO GO: %d\n", tabtogo);
  }

  return tabtogo;
}

void
notebookPage_removed(GtkNotebook* self, GtkWidget* child, guint page_num, gpointer user_data)
{
  (void)child;
  (void)page_num;
  (void)user_data;

  if (g_bkp_current_page == -1) //User was in the tab that was closed!!!
  {
    gint tabToGo = tabstack_pop(self);
    gtk_notebook_set_current_page(self, tabToGo);
  }
}
