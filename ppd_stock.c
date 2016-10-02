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
   if (!stock_added && current == NULL) {
      last->next = new_node;
      stock_added = TRUE;
   }
   if (!stock_added) {
      printf("Returning without adding\n");
   } else {
      system->item_list->count++;
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
   if (stock_removed) {
      system->item_list->count--;
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
   int max_id = 1, i, current_id, x;
   char id[IDLEN];
   struct ppd_node *current = NULL;
   BOOLEAN int_success, not_a_zero_found;

   current = system->item_list->head;

   while (current != NULL) {
      not_a_zero_found = TRUE;
      x = 0;
      for (i = 1; i < IDLEN; i++) {
         if (not_a_zero_found) {
            if (current->data->id[i] != 0)
               id[x] = current->data->id[i];
            not_a_zero_found = FALSE;
            x++;
         } else {
            id[x] = current->data->id[i];
            x++;
         }
      }

      int_success = to_int(id, &current_id);
      if (!int_success) {
         fprintf(stderr, "Invalid ID transform\n");
      }
      if (current_id > max_id) {
         max_id = current_id;
      }
      current = current->next;
   }

   return max_id + 1;
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

BOOLEAN stock_to_string(char string[FILE_LINE_LEN], struct ppd_node *node) {
   struct ppd_stock *stock;
   if (node == NULL) {
      return FALSE;
   } else {
      stock = node->data;
      sprintf(string, "%s|%s|%s|%u.%2.2u|%u", stock->id, stock->name,
              stock->desc,
              stock->price.dollars, stock->price.cents, stock->on_hand);
      return TRUE;
   }
}

BOOLEAN save_stock(struct ppd_system *system) {
   struct ppd_node *node = NULL;
   BOOLEAN no_error = TRUE;
   char output_string[FILE_LINE_LEN];
   FILE *stock_file = NULL, *error_file = NULL;


   node = system->item_list->head;

   rename_file(system->stock_file_name, FALSE);

   error_file = fopen("error.dat", "w");
   stock_file = fopen(system->stock_file_name, "w");
   if (stock_file != NULL && error_file != NULL) {
      while (node != NULL && no_error) {
         no_error = stock_to_string(output_string, node);
         if (no_error) {
            fprintf(stock_file, "%s\n", output_string);
         } else {
            fprintf(error_file, "%s\n", output_string);
         }
         node = node->next;
      }
      fflush(error_file);
      fflush(stock_file);
   }
   if (ftell(error_file) != 0) {
      printf("Errors were encountered when saving. Check error.dat for lines "
                     "that errored\n");
      return FALSE;
   }

   if (error_file != NULL) {
      fclose(error_file);
   } else {
      printf("Error file could not be written to.\n Nothing was saved\n");
      rename_file(system->stock_file_name, TRUE);
      return FALSE;
   }
   if (stock_file != NULL) {
      fclose(stock_file);
   } else {
      printf("Stock file could not be written to. Nothing was saved.\n");
      rename_file(system->stock_file_name, TRUE);
      return FALSE;
   }


   return TRUE;
}

void print_stock(struct ppd_stock stock_item) {
   printf("ID: %s\n", stock_item.id);
   printf("Name: %s\n", stock_item.name);
   printf("Desc: %s\n", stock_item.desc);
   printf("On Hand: %u\n", stock_item.on_hand);
   printf("Price: %u.%2.2u\n----------\n", stock_item.price.dollars,
          stock_item.price
                  .cents);
}

BOOLEAN init_node(struct ppd_node **node, struct ppd_system *system) {
   if (system->item_list->head != NULL) {
      *node = system->item_list->head;
      return TRUE;
   } else {
      return FALSE;
   }
}

BOOLEAN next_node(struct ppd_node **node) {
   if ((*node)->next != NULL) {
      *node = (*node)->next;
      return TRUE;
   } else {
      return FALSE;
   }
}

BOOLEAN is_valid_id(char *id, struct ppd_system *system) {
   int i;
   struct ppd_node *node = NULL;
   if (strlen(id) != IDLEN) {
      return FALSE;
   }
   if (id[0] != 'I') {
      return FALSE;
   }
   for (i = 1; i < IDLEN; i++) {
      if (id[i] < '0' || id[i] > '9') {
         return FALSE;
      }
   }
   if (init_node(&node, system)) {
      do {
         if (strcmp(node->data->id, id) == 0) {
            return FALSE;
         }

      } while (next_node(&node));


   }
   return TRUE;
}