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

/**
 * @file ppd_stock.c this is the file where you will implement the
 * interface functions for managing the stock list.
 **/

BOOLEAN add_stock(ppd_stock stock, ppd_system *system) {
   int i;
   ppd_list *list = NULL;
   ppd_node *new_node = malloc(sizeof(ppd_node));
   ppd_node *current = NULL, *last = NULL;
   BOOLEAN stock_added = FALSE;

   new_node->data = stock;
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
   }

   return stock_added;
}


BOOLEAN remove_stock(ppd_system *system, char id[IDLEN + 1]) {
   int i;
   ppd_list *list = NULL;
   ppd_node *current = NULL, *last = NULL;
   BOOLEAN stock_added = FALSE;

   new_node->data = stock;
   new_node->next = NULL;

   current = system->item_list->head;

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
   }

   return stock_added;
}

BOOLEAN init_list(ppd_system *system) {
   ppd_list *list =NULL;
   ppd_node *node = NULL;

   node->data = NULL;
   node->next = NULL;
   list->count = 0;
   list->head = node;

   system->item_list = list;
   return TRUE;
}

int get_largest_description(ppd_system *system) {
   int longest_desc = 0, current_len;
   ppd_node *current =NULL;

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

int get_next_id(ppd_system *system){
   int max_id = 1;
   ppd_node *current = NULL;


   return get_next_id(system);
}