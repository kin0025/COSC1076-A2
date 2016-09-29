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

   *new_node->data = stock;
   new_node->next = NULL;


   current = system->item_list->head;

   if (current == NULL) {
      system->item_list->head = new_node;
      stock_added = TRUE;
   }

   while (!stock_added && current != NULL) {
      if (strcmp(stock.name, current->data->name) <= 0) {
         if (i == 0) {
            system->item_list->head = new_node;
         } else {
            last->next = new_node;
         }
         new_node->next = current;
         stock_added = TRUE;
      }
      last = current;
      current = current->next;
      i++;
   }
   if (!stock_added && current != NULL && current->next == NULL) {
      current->next = new_node;
      stock_added= TRUE;
   }
   if (!stock_added) {
      printf("Returning without adding\n");
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
   struct ppd_list *list = malloc(sizeof(struct ppd_list));
   list->count = 0;
   list->head = NULL;

   system->item_list = list;
   return TRUE;
}

int get_largest_description(struct ppd_system *system) {
   int longest_desc = 0, current_len;
   struct ppd_node *current = NULL;

   current = system->item_list->head;

   while (current != NULL) {
      current_len = strlen(current->data->desc);
      if (longest_desc < current_len) {
         longest_desc = current_len;
      }
      current = current->next;
   }
   return longest_desc;
}

int get_largest_name(struct ppd_system *system) {
   int longest_name = 0, current_len;
   struct ppd_node *current = NULL;

   current = system->item_list->head;

   while (current != NULL) {
      current_len = strlen(current->data->name);
      if (longest_name < current_len) {
         longest_name = current_len;
      }
      current = current->next;
   }
   return longest_name;
}

struct ppd_node *find_id(struct ppd_node *node, char *id) {

   if (node == NULL) {
      return NULL;
   }
   if (strcmp(node->data->id, id) == 0) {
      return node;
   }
   return find_id(node->next, id);


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

BOOLEAN to_string(char string[FILE_LINE_LEN], struct ppd_node *node) {
   struct ppd_stock *stock;
   if (node == NULL) {
      return FALSE;
   } else {
      stock = node->data;
      sprintf(string, "%s|%s|%s|%u.%2.2u|%u", stock->id, stock->name,
              stock->desc,
              stock->price.dollars, stock->price.dollars, stock->on_hand);
      return TRUE;
   }
}

struct ppd_node *next_node(struct ppd_node *node) {
   return node->next;
}
