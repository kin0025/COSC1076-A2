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
   system->item_list = NULL;
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
   FILE *data_file;
   char *token = NULL, *current_item = NULL;
   ppd_stock *stock_item;

   data_file = fopen(filename,'r');

   current_item =
   token = strtok(current_item, DATA_DELIMITER);
   while(token != NULL) {
      stock_item->id = *token;
      token = strtok(NULL, DATA_DELIMITER);
      stock_item->name = *token;
      token = strtok(NULL, DATA_DELIMITER);
      stock_item->desc = *token;
      token = strtok(NULL, DATA_DELIMITER);
      stock_item->price = *token;
      token = strtok(NULL, DATA_DELIMITER);
      stock_item->on_hand = *token;

      token = strtok(current_item, DATA_DELIMITER);
   }

   /*
    * Please delete this default return value once this function has
    * been implemented. Please note that it is convention that until
    * a function has been implemented it should return FALSE
    */
   return FALSE;
}

/**
 * loads the contents of the coins file into the system. This needs to
 * be implemented as part 1 of requirement 18.
 **/
BOOLEAN load_coins(struct ppd_system *system, const char *) {
   if (system->coin_from_file == TRUE) {
      FILE coin_file = fopen(system->coin_file_name);

   } else {
      void_balances(system->cash_register);
   }
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

}

/*Reused Partially from Assignment 1 */
BOOLEAN read_file_input(char *buffer, int length , FILE *file) {
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