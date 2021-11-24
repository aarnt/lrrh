#include "tabstack.h"

struct tabstack *prev = NULL;
struct tabstack *top  = NULL;

/*int main()
{
  int n, ch;
  do
  {
    printf("\n\nStack Menu\n1. Push \n2. Pop\n3. Display\n0. Exit");
    printf("\nEnter Choice 0-3? : ");
    scanf("%d", &ch);
    switch (ch)
    {
    case 1:
      printf("\nEnter number ");
      scanf("%d", &n);
      push(n);
      break;
    case 2:
      pop();
      break;
    case 3:
      display();
      break;
    }
  }while (ch != 0);
}*/

/*
 * Stacks the given tab widget visited by the user
 */
void tabstack_push(GtkWidget *item)
{
  struct tabstack *nptr = malloc(sizeof(struct tabstack));
  nptr->data = item;

  printf("Pushed gtkWidget: 0x%" PRIXPTR "\n", (uintptr_t)nptr->data);
  nptr->prev = top;
  prev = top;
  nptr->next = top;
  //nptr->isRemoved = FALSE;
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
    /*if (temp->data != NULL) //isRemoved)
    {
      tabtogo = gtk_notebook_page_num(notebook, temp->data);
      top = top->next;
      return tabtogo;
    }*/

    while (temp != NULL)
    {
      //printf("Stack item is NULL!!!\n");
      if (temp->data == NULL)
      {
        prev = top;
        top = top->next;
        temp = top;
      }
      else break;
    }

    //printf("Almost done in POP!\n");
    printf("Planing to focus gtkWidget: 0x%" PRIXPTR "\n", (uintptr_t)temp->data);
    tabtogo = gtk_notebook_page_num(notebook, temp->data);
    printf("TAB TO GO: %d\n", tabtogo);
    //top = top->next;
  }

  return tabtogo;
}

/*
 * Marks given itemToRemove item with a "isRemoved" = TRUE on the stack
 */
void tabstack_remove(GtkWidget *itemToRemove)
{
  struct tabstack *temp, *bkpTop;
  bkpTop = top;
  temp = top;

  while (temp != NULL)
  {
    if (temp->data == itemToRemove)
    {
      //printf("Tentando remover GtkWidget: 0x%" PRIXPTR "\n", (uintptr_t)itemToRemove);
      //printf("Data eh: 0x%" PRIXPTR "\n", (uintptr_t)temp->data);
      //temp->isRemoved = TRUE;
    }

    temp = temp->next;
  }

  top = bkpTop;
}

void
notebookPage_removed(GtkNotebook* self, GtkWidget* child, guint page_num, gpointer user_data)
{
  (void)child;
  (void)page_num;
  (void)user_data;

  //tabstack_remove(child);

  if (g_bkp_current_page == -1) //User was in the tab that was closed!!!
  {
    gint tabToGo = tabstack_pop(self);
    gtk_notebook_set_current_page(self, tabToGo);
  }
}
