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

void read_rest_of_file_line(FILE *file) {
   int ch;
   /* keep retrieving characters from stdin until we
    * are at the end of the buffer
    */
   while (ch = getc(file), ch != '\n' && ch != EOF);
   /* reset error flags on stdin */
   clearerr(file);
}

/**
 * @param system a pointer to a @ref ppd_system struct that holds all 
 * the data for the system we are creating
 **/
BOOLEAN system_init(struct ppd_system *system) {
   void_balances(system->cash_register);
   init_list(system);
   /*
     * Please delete this default return value once this function has 
     * been implemented. Please note that it is convention that until
     * a function has been implemented it should return FALSE
     */
   return TRUE;
}

/**
 * loads the stock file's data into the system. This needs to be 
 * implemented as part of requirement 2 of the assignment specification.
 **/
BOOLEAN load_stock(struct ppd_system *system, const char *filename) {
   FILE *data_file = NULL;
   char *token = NULL, *price, current_line[FILE_LINE_LEN + EXTRACHARS];
   struct ppd_stock stock_item;
   int onhand, line_number = 0;
   BOOLEAN no_error, stock_added;


   data_file = fopen(system->stock_file_name, "r");

   if (data_file == NULL) {
      printf("Failed to load data file. Please close any programs that may have"
                     " it open and try again");
      return FALSE;
   }

   while (read_file_input(current_line, FILE_LINE_LEN, data_file) &&
          !feof(data_file)) {
      line_number++;
      no_error = TRUE;
      if (count_delims(DATA_DELIMITER, current_line) == NUMBER_STOCK_DELIMS) {
         /* Read the first token */
         token = strtok(current_line, DATA_DELIMITER);
         if (token == NULL) {

            return file_error_message("ID", line_number,
                                      system->stock_file_name, data_file);
         }
         /* Set the ID to the first token. Length checking is performed*/
         if (is_valid_id(token, system)) {
            strcpy(stock_item.id, token);
         } else {
            return file_error_message("ID", line_number,
                                      system->stock_file_name, data_file);
            continue;
         }
         token = strtok(NULL, DATA_DELIMITER);
         if (token == NULL) {
            return file_error_message("Name", line_number,
                                      system->stock_file_name, data_file);
         }
         if (strlen(token) <= NAMELEN) {
            strcpy(stock_item.name, token);
         } else {
            return file_error_message("Name", line_number,
                                      system->stock_file_name, data_file);
            continue;

         }

         token = strtok(NULL, DATA_DELIMITER);
         if (token == NULL) {
            return file_error_message("Description", line_number,
                                      system->stock_file_name, data_file);
         }
         if (strlen(token) <= DESCLEN) {
            strcpy(stock_item.desc, token);
         } else {
            return file_error_message("Description", line_number,
                                      system->stock_file_name, data_file);
            continue;
         }

         price = strtok(NULL, DATA_DELIMITER);
         if (price == NULL) {
            return file_error_message("Price", line_number,
                                      system->stock_file_name, data_file);
         }

         /*This has to go before the string to price, as strtok is not
          * reentrant, and string to price uses it */
         token = strtok(NULL, DATA_DELIMITER);
         if (token == NULL) {

            return file_error_message("Stock Quantity", line_number,
                                      system->stock_file_name, data_file);
         }
         no_error = to_int(token, &onhand);

         /* Check that the integer conversion went successfully */
         if (onhand < 0) {
            no_error = FALSE;
         } else if (!no_error) {

            return file_error_message("Stock Quantity", line_number,
                                      system->stock_file_name, data_file);
            continue;
         } else {
            stock_item.on_hand = onhand;
         }

         no_error = string_to_price(&(stock_item.price), price);

         if (!no_error) {

            return file_error_message("Price", line_number,
                                      system->stock_file_name, data_file);
         }



/*
      print_stock(stock_item);
*/

         if (no_error) {
            stock_added = add_stock(stock_item, system);
            if (!stock_added) {
               printf("Error encountered and stock could not be added. Please "
                              "try again, or check your file syntax.\n");
               fclose(data_file);
               return FALSE;
            }
         }
      } else {
         printf("Line %d has wrong number of fields:  %s\n",
                line_number, current_line);
         fclose(data_file);
         return FALSE;
      }
   }
   /* Checking for a final line that does not end with a new line. Will not
    * be tokenised, so we current line will not have \0 added during
    * tokenisation
    */
   if (strlen(current_line) > IDLEN) {
      fclose(data_file);
      printf("The last line of %s does not end in a new line, and may be "
                     "indicative of file corruption. Loading will halt "
                     "until file is fixed\n", system->stock_file_name);
      return FALSE;
   }
   if (!feof(data_file)) {
      fclose(data_file);
      return FALSE;
   }
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
   char current_line[COIN_LINE_LEN + EXTRACHARS], *token;
   int denom_value, amount, line_number = 0;

   void_balances(system->cash_register);

   if (system->coin_from_file == TRUE) {
      coin_file = fopen(system->coin_file_name, "r");
      if (coin_file != NULL) {

         while (!feof(coin_file) && read_file_input(current_line, COIN_LINE_LEN,
                                                    coin_file)) {
            line_number++;

            if (count_delims(COIN_DELIM, current_line) == 1) {
               token = strtok(current_line, COIN_DELIM);
               if (token == NULL) {
                  return file_error_message("Coin Type", line_number,
                                            system->coin_file_name, coin_file);
               }

               if (!to_int(token, &denom_value) ||
                   !is_valid_value(denom_value)) {
                  return file_error_message("Coin Type", line_number,
                                            system->coin_file_name, coin_file);
               }
               token = strtok(NULL, COIN_DELIM);
               if (token == NULL) {
                  return file_error_message("Coin Quantity", line_number,
                                            system->coin_file_name, coin_file);
               }

               if (!to_int(token, &amount) || amount < 0) {
                  return file_error_message("Coin Quantity", line_number,
                                            system->coin_file_name, coin_file);
               }
               if (count_coins(system->cash_register, value_to_denom
                       (denom_value)) != 0) {
                  return file_error_message("Coin: duplicate - already added",
                                            line_number,
                                            system->coin_file_name, coin_file);

               }

               if (!add_coin_val(system->cash_register, denom_value, amount)) {
                  printf("Coin adding on line %d failed. Try loading again, "
                                 "and check file syntax", line_number);
                  return FALSE;
               }
            } else {
               system->coin_from_file = FALSE;
               return file_error_message("Number of fields",
                                         line_number,
                                         system->coin_file_name, coin_file);
            }

         }
         /* Checking for a non new line terinated file */
         if (count_delims(COIN_DELIM, current_line) != 0) {
            fclose(coin_file);
            printf("The last line of %s does not end in a new line, and may be "
                           "indicative of file corruption. Loading will halt "
                           "until file is fixed\n", system->coin_file_name);
            return FALSE;
         }
         if (!feof(coin_file)) {
            fclose(coin_file);

            return FALSE;
         }
         fclose(coin_file);

      } else {
         printf("Invalid coin file. No coins were loaded.\n");
         return FALSE;
      }
   }

   return TRUE;

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

      if (fgets(buffer, length + 1, file) == NULL || feof(file)) {
         return FALSE;
      }

      /* Check for overflow and deal with it */
      if (buffer[strlen(buffer) - 1] != '\n') {
         overflow = TRUE;
         /* Clear the overflow and prompt the user for input again */
         read_rest_of_file_line(file);
         printf("One of your lines in the file is longer then %d characters."
                        "\n", length);
         return FALSE;
      } else {
         overflow = FALSE;
      }
      if (buffer[0] == EOF) { return FALSE; }

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
/** Returns false if user presses enter or EOF. Enters input into the buffer
 * file, with expected length of length*/
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


BOOLEAN read_int(int *output) {
   char buffer[MAX_INT_LEN + EXTRACHARS];
   char *ptr = NULL;
   BOOLEAN running = TRUE;
   do {
      /* read the input */
      if (fgets(buffer, MAX_INT_LEN + EXTRACHARS, stdin) == NULL) {
         return FALSE;
      }
      /* check if we overflowed the buffer and fix it if we did*/
      if (buffer[strlen(buffer) - 1] != '\n') {
         read_rest_of_line();
         printf("Input too long, please try again\n");
         running = TRUE;
         continue;
      }
      /* Remove the extra null terminator */
      buffer[strlen(buffer) - 1] = NULL_TERMINATOR;

      if (strlen(buffer) == 0) { return FALSE; }

      /* Convert the remaining to integer */
      *output = (int) strtol(buffer, &ptr, BASE);

      /* Check that the integer conversion went successfully */
      if (*output == -1 || ptr == buffer) {
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
   return TRUE;
}

BOOLEAN to_int(char *input, int *output) {
   char *ptr = NULL;
   BOOLEAN no_error = TRUE;
   /* Convert the remaining to integer */
   *output = (int) strtol(input, &ptr, BASE);

   /* Check that the integer conversion went successfully */
   if (*output == -1 || ptr == input || strlen(ptr) != 0) {
      no_error = FALSE;

   }
   return no_error;
}

BOOLEAN string_to_price(struct price *price_amount, char *price_input) {
   char *ptr = NULL;
   int amount;
   BOOLEAN int_success;

   ptr = strtok(price_input, PRICEDELIM);
   int_success = to_int(ptr, &amount);
   if (ptr != NULL && int_success && amount < MAX_DOLLARS_PRICE) {
      price_amount->dollars = amount;
   } else {
      return FALSE;
   }
   /* strtok_r would make this easier/safer if the calling function also uses
    * strtok, however not C99 */
   ptr = strtok(NULL, PRICEDELIM);
   int_success = to_int(ptr, &amount);

   if (ptr != NULL && int_success && amount < 100 && (amount % MINUMUM_DENOM)
                                                     == 0) {
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

BOOLEAN price_to_int(struct price *price, int *cents) {
   if (price == NULL) {
      return FALSE;
   }
   *cents = price->cents + (price->dollars * CENTS_IN_DOLLAR);
   return TRUE;
}

BOOLEAN rename_file(const char *name, BOOLEAN reverse) {
   char *file_bak = NULL;
   BOOLEAN worked;

   file_bak = malloc(sizeof(char) * (strlen(name) + 3));
   sprintf(file_bak, "%s.bak", name);

   if (reverse) {
      worked = rename(file_bak, name);
   } else {
      worked = rename(name, file_bak);
   }
   free(file_bak);

   return worked;
}

/* Only functions on single chatacter delimiters */
int count_delims(char *delim, char *string) {
   if (strlen(delim) != 1)return -1;
   if (*string == NULL_TERMINATOR) {
      return 0;
   }
   if (*string == delim[0]) {
      return 1 + count_delims(delim, ++string);
   }
   return count_delims(delim, ++string);
}

BOOLEAN
file_error_message(char *field_type, int line_number, const char *file_name,
                   FILE *file) {
   fprintf(stderr, "Invalid %s%s%s when loading line %s%d%s in file %s. Item "
                   "was not added\n",
           F_CYAN, field_type, COLOUR_RESET, F_CYAN, line_number, COLOUR_RESET,
           file_name);
   fclose(file);
   return FALSE;
}