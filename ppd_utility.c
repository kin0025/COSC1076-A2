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

#include "ppd_utility.h"

/**
 * @file ppd_utility.c contains implementations of important functions for
 * the system. If you are not sure where to implement a new function, 
 * here is probably a good spot.
 **/

void read_rest_of_line(void) {
   int ch;
   /* keep retrieving characters from stdin until we
    * are at the end of the buffer
    */
   while (ch = getc(stdin), ch != '\n' && ch != EOF);
   /* reset error flags on stdin */
   clearerr(stdin);
}

/**
 * @param system a pointer to a @ref ppd_system struct that holds all 
 * the data for the system we are creating
 **/
BOOLEAN system_init(struct ppd_system *system) {
   void_balances(system->cash_register);
   printf("Initialising List\n");
   init_list(system);
   printf("LISTDONE");
   /*
     * Please delete this default return value once this function has 
     * been implemented. Please note that it is convention that until
     * a function has been implemented it should return FALSE
     */
   return FALSE;
}

/**
 * loads the stock file's data into the system. This needs to be 
 * implemented as part of requirement 2 of the assignment specification.
 **/
BOOLEAN load_stock(struct ppd_system *system, const char *filename) {
   FILE *data_file = NULL;
   char *token = NULL, *price, current_line[FILE_LINE_LEN + EXTRACHARS];
   struct ppd_stock stock_item;
   int onhand;
   BOOLEAN no_error = TRUE;


   data_file = fopen(system->stock_file_name, "r");

   if (data_file == NULL) {
      printf("Failed to load data file. Please close any programs that may have "
                     "it open and try again");
      return FALSE;
   }
   
   while (read_file_input(current_line, FILE_LINE_LEN, data_file)) {
      printf("%s", current_line);
      token = strtok(current_line, DATA_DELIMITER);

      strcpy(stock_item.id, token);

      token = strtok(NULL, DATA_DELIMITER);

      strcpy(stock_item.name, token);

      token = strtok(NULL, DATA_DELIMITER);

      strcpy(stock_item.desc, token);

      token = strtok(NULL, DATA_DELIMITER);

      price = token;

      no_error = string_to_price(&(stock_item.price), price);

      token = strtok(token, DATA_DELIMITER);

      no_error = to_int(token, &onhand);
      /* Check that the integer conversion went successfully */
      if (onhand < 0) {
         no_error = FALSE;
      } else {
         stock_item.on_hand = onhand;
      }
      printf("%s\n", stock_item.id);
      printf("%s\n", stock_item.desc);
      printf("%s\n", stock_item.name);
      printf("%d\n", stock_item.on_hand);
      printf("%d.%d\n----------\n", stock_item.price.dollars,
             stock_item.price
                     .cents);
      if (no_error) {
         printf("Adding Item\n");
         add_stock(stock_item, system);
         printf("ADD DONE\n");
      }
      no_error = TRUE;

   }
   printf("ADD DONE");

   fclose(data_file);

   /*
    * Please delete this default return value once this function has
    * been implemented. Please note that it is convention that until
    * a function has been implemented it should return FALSE
    */
   return TRUE;
}

/**
 * loads the contents of the coins file into the system. This needs to
 * be implemented as part 1 of requirement 18.
 **/
BOOLEAN load_coins(struct ppd_system *system, const char *filename) {
   FILE *coin_file;
   if (system->coin_from_file == TRUE) {
      coin_file = fopen(system->coin_file_name, "r");
/*todo: Add coin loading properly */
   } else {
      void_balances(system->cash_register);
   }
   fclose(coin_file);
   /*
    * Please delete this default return value once this function has
    * been implemented. Please note that it is convention that until
    * a function has been implemented it should return FALSE
    */
   return FALSE;

}

/**
 * @param system a pointer to a @ref ppd_system struct that holds all 
 * the data for the system we are creating
 **/
void system_free(struct ppd_system *system) {
   struct ppd_node *current, *next;
   current = system->item_list->head;
   while (current != NULL) {
      next = current->next;
      del_node(current);
      current = next;
   }
   free(system->item_list);
}

/*Reused Partially from Assignment 1 */
BOOLEAN read_file_input(char *buffer, int length, FILE *file) {
   BOOLEAN overflow = FALSE;
   do {
      /* Check for EOF input and return false, receive input */
      if (fgets(buffer, length + 1, file) == NULL) {
         return FALSE;
      }

      /* Check for overflow and deal with it */
      if (buffer[strlen(buffer) - 1] != '\n') {
         overflow = TRUE;
         /* Clear the overflow and prompt the user for input again */
         read_rest_of_line();
         printf("One of your lines in the file is longer then %d characters, "
                        "and has been "
                        "discarded.\n", length);
      } else {
         overflow = FALSE;
      }
   } while (overflow);
   /* Remove the EOL character from string */
   buffer[strlen(buffer) - 1] = NULL_TERMINATOR;
   /*If there was only an EOL character return false */
   if (strlen(buffer) == 0) {
      return FALSE;
   }
   return TRUE;
}

/*Reused Partially from Assignment 1 */
BOOLEAN read_user_input(char *buffer, int length) {
   BOOLEAN overflow = FALSE;
   do {
      /* Check for EOF input and return false, receive input */
      if (fgets(buffer, length + EXTRACHARS, stdin) == NULL) {
         return FALSE;
      }

      /* Check for overflow and deal with it */
      if (buffer[strlen(buffer) - 1] != '\n') {
         overflow = TRUE;
         /* Clear the overflow and prompt the user for input again */
         read_rest_of_line();
         printf("Your input has to be less than %d characters long. Please"
                        " try again\n", length);
      } else {
         overflow = FALSE;
      }
   } while (overflow);
   /* Remove the EOL character from string */
   buffer[strlen(buffer) - 1] = NULL_TERMINATOR;
   /*If there was only an EOL character return false */
   if (strlen(buffer) == 0) {
      return FALSE;
   }
   return TRUE;
}

/* Returns true if the first name is earlier in the alphabet than the second */
BOOLEAN name_sort(char first[NAMELEN + 1], char second[NAMELEN + 1]) {
   BOOLEAN match = TRUE;
   BOOLEAN is_smaller = TRUE;
   int i = 0;

   while (match && i < NAMELEN) {
      if (first[i] != second[i]) {
         match = FALSE;
      } else if (first[i] < second[i]) {
         match = FALSE;
         is_smaller = TRUE;
      }
      i++;
   }
   if (match) {
      return match;
   } else {
      return is_smaller;
   }

}

int read_int(void) {
   char buffer[MAX_INT_LEN + EXTRACHARS];
   int output;
   char *ptr = NULL;
   BOOLEAN running = TRUE;
   do {
      /* read the input */
      fgets(buffer, MAX_INT_LEN, stdin);
      /* check if we overflowed the buffer and fix it if we did*/
      if (buffer[strlen(buffer) - 1] != '\n') {
         read_rest_of_line();
         printf("Input too long, please try again\n");
         running = TRUE;
         continue;
      }
      /* Remove the extra null terminator */
      buffer[strlen(buffer) - 1] = NULL_TERMINATOR;

      /* Convert the remaining to integer */
      output = (int) strtol(buffer, &ptr, BASE);

      /* Check that the integer conversion went successfully */
      if (output == -1 || ptr == buffer) {
         printf("The input was not a parsable number.\n");
         running = TRUE;
      } else if (strlen(ptr) != 0) {
         printf("There was more than just a number entered, please try "
                        "again.\n");
         running = TRUE;
      } else {
         running = FALSE;
      }
   } while (running);
   return output;
}

BOOLEAN to_int(char *input, int *output) {
   char *ptr;
   BOOLEAN no_error = TRUE;
   /* Convert the remaining to integer */
   *output = (int) strtol(input, &ptr, BASE);

   /* Check that the integer conversion went successfully */
   if (*output == -1 || ptr == input) {
      printf("The input was not a parsable number: %s\n", input);
      no_error = FALSE;
   } else if (strlen(ptr) != 0) {
      printf("There was more than just a number entered, please try "
                     "again.\n");
      no_error = FALSE;
   }
   return no_error;
}

BOOLEAN string_to_price(struct price *price_amount, char *price_input) {
   char *ptr;
   int amount;
   BOOLEAN int_success;

   ptr = strtok(price_input, PRICEDELIM);
   int_success = to_int(ptr, &amount);
   if (ptr != NULL && int_success) {
      price_amount->dollars = amount;
   } else {
      printf("Failed");
      return FALSE;
   }
   ptr = strtok(NULL, PRICEDELIM);
   int_success = to_int(ptr, &amount);

   if (ptr != NULL && int_success) {
      price_amount->cents = amount;
   } else {
      return FALSE;
   }

   ptr = strtok(NULL, PRICEDELIM);


   if (ptr == NULL) {
      return TRUE;
   } else {
      return FALSE;
   }
}