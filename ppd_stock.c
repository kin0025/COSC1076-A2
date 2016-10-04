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

/**
 * Adding item stock to the item_list enclosed in system in alphavetical order.
 * @param stock A populated stock item to be added to the list
 * @param system The system to add the item to
 * @return true if the item was added, false if any error was encountered
 * whilst adding.
 */
BOOLEAN add_stock(struct ppd_stock stock, struct ppd_system *system) {
   int i = 0;
   struct ppd_node *new_node = NULL;
   struct ppd_node *current = NULL, *last = NULL;
   BOOLEAN stock_added = FALSE;

/* Create and malloc the new node */
   new_node = create_node();

   /* If the malloc failed we can assume free was run already, so return an
    * error message */
   if (new_node == NULL) {
      printf("Memory Allocation failed, Item was not added.");
      return FALSE;
   }

   *new_node->data = stock;
   new_node->next = NULL;


   /* If the firs entry in the list is NULL (init_node reurns false), set the
    * first node to the new one, as the list is empty */
   if (!init_node(&current, system)) {
      system->item_list->head = new_node;
      stock_added = TRUE;
   }

   /* If stock hasn't been added already */
   if (!stock_added) {
      do {
         /* Sort the stock based on strcmp. If the result is less then 0 it
          * is earlier in the alphabet*/
         if (strcmp(stock.name, current->data->name) <= 0) {
            /* If it is the first item make the head equal the new node*/
            if (i == 0) {
               system->item_list->head = new_node;
            } else {
               last->next = new_node;
            }
            /* Put the current node in the position after the added node as
             * it is sorted directly after it */
            new_node->next = current;
            stock_added = TRUE;
         }
         last = current;
         i++;
      } while (next_node(&current) && !stock_added);
   }

   /* If stock was not added throughout the list the item must fit at the
    * very end*/
   if (!stock_added && current == NULL) {
      last->next = new_node;
      stock_added = TRUE;
   }
   /* If we didn't add the stock for some reason free the memory and quit*/
   if (!stock_added) {
      printf("Returning without adding\n");
      del_node(new_node);
   } else {
      system->item_list->count++;
   }
   return stock_added;
}

/**
 * Removes stock from based on an ID given
 * @param system The system that contains the item_list that we are searching in
 * @param id The id that we are searching for
 * @return Whether the item was found and deleted
 */
BOOLEAN remove_stock(struct ppd_system *system, char id[IDLEN + 1]) {
   int i = 0;
   struct ppd_node *current = NULL, *last = NULL;

   /* Init the starting node, and if it fails return false */
   if (!init_node(&current, system)) {
      return FALSE;
   }
   /* Loop through all the nodes*/
   do {
      /* If the current node has the same ID as we are searching for delete it*/
      if (strcmp(current->data->id, id) == 0) {
         /* If we are deleting the first node make sure the head is still set */
         if (i == 0) {
            system->item_list->head = current->next;
         } else {
            last->next = current->next;
         }
         /* Remove the memory allocated for the node once it is removed from
          * the list*/
         del_node(current);
         /* Decrease the item count */
         system->item_list->count--;
         return TRUE;
      }
      last = current;
      i++;
   } while (next_node(&current));
   /* If we make it to the bottom of the function then item was not removed */
   return FALSE;
}

/**
 * Initialises the list to empty values
 * @param system The system that contains the list to be initialised
 * @return whether the list was properly initialised
 */
BOOLEAN init_list(struct ppd_system *system) {
   /* Malloc the list and set it*/
   struct ppd_list *list = malloc(sizeof(struct ppd_list));
   list->count = 0;
   list->head = NULL;

   system->item_list = list;
   return TRUE;
}

/**
 * Returns the length of the largest description in the system's list
 * @param system The system that contains the list
 * @return the length of the larges object
 */
int get_largest_description(struct ppd_system *system) {
   int longest_desc = 0, current_len;
   struct ppd_node *current = NULL;

   /* If initialisation fails return 0; */
   if (!init_node(&current, system))
      return 0;

   /* Loop through all the items in the list*/
   do {
      current_len = strlen(current->data->desc);
      /* If the length is linger than the previous maximum make this one the
       * max*/
      if (longest_desc < current_len) {
         longest_desc = current_len;
      }
   } while (next_node(&current));
   /* return the number */
   return longest_desc;
}

/**
 * Returns the length of the largest name in the system's list
 * @param system The system that contains the list
 * @return the length of the largest object
 */
int get_largest_name(struct ppd_system *system) {
   int longest_name = 0, current_len;
   struct ppd_node *current = NULL;

   if (!init_node(&current, system))
      return 0;

   /* Loop through all the items in the list*/
   do {
      current_len = strlen(current->data->name);
      /* If the length is linger than the previous maximum make this one the
       * max*/
      if (longest_name < current_len) {
         longest_name = current_len;
      }
   } while (next_node(&current));

   /* return the maximum */
   return longest_name;
}

/**
 * Finds the stock that owns the ID and returns it
 * @param system The system item that contains the item list to search through
 * @param id The id to search for
 * @return NULL if nothing found. The found stock item
 */
struct ppd_stock *find_id(struct ppd_system *system, char *id) {
   struct ppd_node *node;

   /* If init fails, return NULL */
   if (!init_node(&node, system))
      return NULL;


   do {
      /* Should never get this as next_node validates */
      if (node == NULL) {
         return NULL;
      }
      /* If the id strings match, return the found node */
      if (strcmp(node->data->id, id) == 0) {
         return node->data;
      }
   } while (next_node(&node));
   return NULL;

}

/**
 * Returns the number of the next ID that hasn't been used
 * @param system The system to search through
 * @return the next unused/never used(assuming the largest ID node never gets
 * deleted, as we have no system to store these) id number
 */
int get_next_id(struct ppd_system *system) {
   int max_id = 1, i, current_id, x;
   char id[IDLEN + EXTRACHARS] = "0";
   struct ppd_node *current = NULL;
   BOOLEAN int_success, not_a_zero_found;

   /* If init fails, there are no nodes, so start from one*/
   if (!init_node(&node, system))
      return 1;
   /* Iterate through all the nodes and analyse their IDs */
   do {
      /* Loop through the current id string and remove all the Zeros and the
       * first character, so str to l with error detection can still function*/
      not_a_zero_found = TRUE;
      x = 0;
      /* Loop through the ID, skipping the first char */
      for (i = 1; i < IDLEN; i++) {
         /* If we haven't found a non-zero char check if the next char is not
          * a zero */
         if (not_a_zero_found) {
            /* If the current char is not a zero start transcribing into the
             * new string */
            if (current->data->id[i] != 0)
               id[x] = current->data->id[i];
            not_a_zero_found = FALSE;
            x++;
         } else {
            /* If we have already found a non-zero character, transcribe all
             * of the rest */
            id[x] = current->data->id[i];
            x++;
         }
      }

      /* Convert the given string into an Int */
      int_success = to_int(id, &current_id);
      if (!int_success) {
         fprintf(stderr, "Invalid ID transform\n");
      }
      /* If the int is larger than the previous max, it is the new max*/
      if (current_id > max_id) {
         max_id = current_id;
      }
   } while (next_node(&current));
   /* The next id is the largest one +1 */
   return max_id + 1;
}

/**
 * Mallocs a node and returns if it worked
 * @return The memory pointer that was malloced
 */
struct ppd_node *create_node(void) {
   /* Malloc the node */
   struct ppd_node *returnVal = malloc(sizeof(struct ppd_node));
   if (returnVal == NULL) {
      return NULL;
   }
   /* Malloc the node's data */
   returnVal->data = malloc(sizeof(struct ppd_stock));
   if (returnVal->data == NULL) {
      free(returnVal);
      return NULL;
   }
   return returnVal;
}

/**
 * Frees the memory of a node
 * @param node The node that needs to have its memory freed. Should have been
 * malloced prior
 */
void del_node(struct ppd_node *node) {

   if (node != NULL) {
      free(node->data);
      free(node);
   }
}

/**
 * Converts a stock item into a delimited, formatted string
 * @param string The string to be output into
 * @param node The node to make the string out of
 * @return Whether a string was made
 */
BOOLEAN stock_to_string(char string[FILE_LINE_LEN], struct ppd_node *node) {
   struct ppd_stock *stock;
   /* If the node is invalid, we can't make a string out of it */
   if (node == NULL) {
      return FALSE;
   } else {
      stock = node->data;
      /* Make a string, using the delimiter as specified*/
      sprintf(string, "%s%s%s%s%s%s%u.%2.2u%s%u",
              stock->id, DATA_DELIMITER,
              stock->name, DATA_DELIMITER,
              stock->desc, DATA_DELIMITER,
              stock->price.dollars, stock->price.cents, DATA_DELIMITER,
              stock->on_hand);
      return TRUE;
   }
}

/**
 * Save all the items to the file located in the system for data
 * @param system Contains the file name to save to and the list to be saved
 * @return Whether the save completed
 */
BOOLEAN save_stock(struct ppd_system *system) {
   struct ppd_node *node = NULL;
   BOOLEAN no_error = TRUE;
   char output_string[FILE_LINE_LEN];
   FILE *stock_file = NULL, *error_file = NULL;

/* If init fails empty the stock file and return true, as there is nothing to
 * save*/
   if (!init_node(&node)) {
      stock_file = fopen(system->stock_file_name, "w");
      fclose(stock_file);
      return TRUE;
   }

   /* Rename the file for backup just in case something goes wrong*/
   rename_file(system->stock_file_name, FALSE);

   /* Open the files we need to write to */
   error_file = fopen("error.dat", "w");
   stock_file = fopen(system->stock_file_name, "w");
   /* If opening didn't fail keep saving */
   if (stock_file != NULL && error_file != NULL) {
      /* Loop for as long as there are more items in the list */
      do {
         /* If the string creation fails leave the loop */
         no_error = stock_to_string(output_string, node);
         /* If there was no error write the items to the stock, otherwise
          * write the offending item to the error file */
         if (no_error) {
            fprintf(stock_file, "%s\n", output_string);
         } else {
            fprintf(error_file, "%s\n", output_string);
         }
      } while (next_node(&node));
      /* probably not needed, but why not */
      fflush(error_file);
      fflush(stock_file);
   }
   /* If stuff was written to the error file, return FALSE and inform the
    * user of the errored lines */
   if (ftell(error_file) != 0) {
      printf("Errors were encountered when saving. Check error.dat for lines "
                     "that errored\n");
      return FALSE;
   }

   /* Check for the file that didn't open and print appropriately. Perform
    * proper cleanup*/
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

/* If no errors were detected the whole way, return true */
   return TRUE;
}

/** Prints a single stock item. No validation needed or occurs*/
void print_stock(struct ppd_stock stock_item) {
   printf("ID: %s\n", stock_item.id);
   printf("Name: %s\n", stock_item.name);
   printf("Desc: %s\n", stock_item.desc);
   printf("On Hand: %u\n", stock_item.on_hand);
   printf("Price: %u.%2.2u\n----------\n", stock_item.price.dollars,
          stock_item.price
                  .cents);
}

/**
 * Initialises a node to the head position of the item_list
 * @param node The node to be initialised to a position
 * @param system The system that contains the list
 * @return Whether the initialisation completed
 */
BOOLEAN init_node(struct ppd_node **node, struct ppd_system *system) {
   if (system->item_list->head != NULL) {
      *node = system->item_list->head;
      return TRUE;
   } else {
      return FALSE;
   }
}

/**
 * Checks for a next node and increments the current node in the lsit if
 * possible.
 * @param node The node to be incremented
 * @return
 */
BOOLEAN next_node(struct ppd_node **node) {
   if (*node == NULL) {
      return FALSE;
   }
   /* IF the next node is NULL return false, otherwise increment */
   if ((*node)->next != NULL) {
      *node = (*node)->next;
      return TRUE;
   } else {
      return FALSE;
   }
}

/**
 * Checks if an ID is syntaxically valid
 * @param id The id to check
 * @param system the system to check for duplicates in
 * @return Whether the ID is valid or not
 */
BOOLEAN is_valid_id(char *id, struct ppd_system *system) {
   int i;
   struct ppd_node *node = NULL;

   /* Check if the ID is correct length */
   if (strlen(id) != IDLEN) {
      return FALSE;
   }
   /* Check if the ID starts with and I */
   if (id[0] != 'I') {
      return FALSE;
   }
   /* Check the remaining characters are number s*/
   for (i = 1; i < IDLEN; i++) {
      if (id[i] < '0' || id[i] > '9') {
         return FALSE;
      }
   }
   /* Check for duplicates */
   if (init_node(&node, system)) {
      do {
         if (strcmp(node->data->id, id) == 0) {
            return FALSE;
         }

      } while (next_node(&node));


   }
   return TRUE;
}