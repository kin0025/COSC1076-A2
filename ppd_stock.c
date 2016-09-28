/***********************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Semester 2 2016 Assignment #2
 * Full Name        : Alexander Chi-Cheng Kinross-Smith
 * Student Number   : s3603437
 * Course Code      : COSC1076
 * Program Code     : BH094
 * Start up code provided by Paul Miller
 * Some codes are adopted here with permission by an anonymous author
 ***********************************************************************/
#include "ppd_stock.h"
#include "ppd_utility.h"

/**
 * @file ppd_stock.c this is the file where you will implement the
 * interface functions for managing the stock list.
 **/

BOOLEAN add_stock(struct ppd_stock stock, struct ppd_system *system) {
   int i = 0;
   struct ppd_node *new_node = NULL;
   struct ppd_node *current = NULL, *last = NULL;
   BOOLEAN stock_added = FALSE;

   new_node = create_node();

   if (new_node == NULL) {
      printf("Memory Allocation failed, Item was not added.");
      return FALSE;
   }

   new_node->data = &stock;
   new_node->next = NULL;

   current = system->item_list->head;

   if (current == NULL) {
      system->item_list->head = new_node;
      stock_added = TRUE;
   }

   while (current->next != NULL && !stock_added) {
      if (name_sort(stock.name, current->data->name)) {
         if (i != 0) {
            last->next = new_node;
         } else {
            system->item_list->head = new_node;
         }
         new_node->next = current;
         stock_added = TRUE;
      }
      last = current;
      current = current->next;
      i++;
   }

   return stock_added;
}


BOOLEAN remove_stock(struct ppd_system *system, char id[IDLEN + 1]) {
   int i = 0;
   struct ppd_node *current = NULL, *last = NULL;
   BOOLEAN stock_removed = FALSE;

   current = system->item_list->head;

   while (current != NULL && !stock_removed) {
      if (strcmp(current->data->id, id) == 0) {
         if (i == 0) {
            system->item_list->head = current->next;
         } else {
            last->next = current->next;
         }
         del_node(current);
         stock_removed = TRUE;
      }
      last = current;
      current = current->next;
      i++;
   }

   return stock_removed;
}

BOOLEAN init_list(struct ppd_system *system) {
   struct ppd_list *list = NULL;
   struct ppd_node *node = NULL;

   node->data = NULL;
   node->next = NULL;
   list->count = 0;
   list->head = node;

   system->item_list = list;
   return TRUE;
}

int get_largest_description(struct ppd_system *system) {
   int longest_desc = 0, current_len;
   struct ppd_node *current = NULL;

   current = system->item_list->head;

   while (current->next != NULL) {
      current_len = strlen(current->data->desc);
      if (longest_desc < current_len) {
         longest_desc = current_len;
      }
      current = current->next;
   }
   return longest_desc;
}

BOOLEAN find_id(struct ppd_node *node, char *id, struct ppd_stock *result) {
   if (node == NULL) {
      return FALSE;
   }

   if (strcmp(node->data->id, id) == 0) {
      result = node->data;
      return TRUE;
   }
   return find_id(node->next, id, result);


}

int get_next_id(struct ppd_system *system) {
   int max_id = 1, i, current_id;
   char id[IDLEN];
   struct ppd_node *current = NULL;
   BOOLEAN int_success;

   current = system->item_list->head;

   while (current != NULL) {
      for (i = 1; i < IDLEN; i++) {
         id[i - 1] = current->data->id[i];
      }
      int_success = to_int(id, &current_id);
      if (!int_success) {
         fprintf(stderr, "Invalid ID transform");
      }
      if (current_id > max_id) {
         max_id = current_id;
      }
      current = current->next;
   }

   return max_id;
}

struct ppd_node *create_node(void) {
   struct ppd_node *returnVal = malloc(sizeof(struct ppd_node));
   if (returnVal == NULL)
      return NULL;


   returnVal->data = malloc(sizeof(struct ppd_stock));
   if (returnVal->data == NULL) {
      free(returnVal);
      return NULL;
   }


   return returnVal;
}

void del_node(struct ppd_node *node) {

   if (node != NULL) {
      free(node->data);
      free(node);
   }
}
