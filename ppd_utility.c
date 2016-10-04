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

/**
 * Wow much fun. Reads the resto fo the line from stdin and discards it
 */
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
 * Reads the rest of the line from file stream and discards it
 * @param file the file stream to read from
 */
void read_rest_of_file_line(FILE *file) {
   int ch;
   /* keep retrieving characters from file until we
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
   return TRUE;
}

/**
 * Loads the stock into the system from the file filename
 * @param system the system to load the stock into
 * @param filename the filename to load the stock from
 * @return Whether the stock was correctly laoded with no errors
 */
BOOLEAN load_stock(struct ppd_system *system, const char *filename) {
   FILE *data_file = NULL;
   char *token = NULL, *price, current_line[FILE_LINE_LEN + EXTRACHARS];
   struct ppd_stock stock_item;
   int onhand, line_number = 0;
   BOOLEAN no_error, stock_added;

/* Open the data file*/
   data_file = fopen(system->stock_file_name, "r");

   /* Validate the data file*/
   if (data_file == NULL) {
      printf("Failed to load data file. Please close any programs that may have"
                     " it open and try again");
      return FALSE;
   }

   /* As long as there are more lines in the file, keep reading them */
   while (read_file_input(current_line, FILE_LINE_LEN, data_file) &&
          !feof(data_file)) {
      /* Increment the line counter for error display */
      line_number++;
      no_error = TRUE;
      /* Check that the line has the correct number of delimiters */
      if (count_delims(DATA_DELIMITER, current_line) == NUMBER_STOCK_DELIMS) {
         /* Read the first token and validate*/
         token = strtok(current_line, DATA_DELIMITER);
         if (token == NULL) {
            /* Close file stream and return an error message - see
             * implementation and function descriptor */
            return file_error_message("ID", line_number,
                                      system->stock_file_name, data_file);
         }
         /* Set the ID to the first token. Validation is performed*/
         if (is_valid_id(token, system)) {
            strcpy(stock_item.id, token);
         } else {
            return file_error_message("ID", line_number,
                                      system->stock_file_name, data_file);
         }

         /* Load the name from the second field */
         token = strtok(NULL, DATA_DELIMITER);
         if (token == NULL) {
            return file_error_message("Name", line_number,
                                      system->stock_file_name, data_file);
         }
         /* Check its length */
         if (strlen(token) <= NAMELEN) {
            strcpy(stock_item.name, token);
         } else {
            return file_error_message("Name", line_number,
                                      system->stock_file_name, data_file);

         }

         /* Load the description from the third field */
         token = strtok(NULL, DATA_DELIMITER);
         if (token == NULL) {
            return file_error_message("Description", line_number,
                                      system->stock_file_name, data_file);
         }
         /* Check the length of the token */
         if (strlen(token) <= DESCLEN) {
            strcpy(stock_item.desc, token);
         } else {
            return file_error_message("Description", line_number,
                                      system->stock_file_name, data_file);
         }

         /* Load the price into a different variable*/
         price = strtok(NULL, DATA_DELIMITER);
         if (price == NULL) {
            return file_error_message("Price", line_number,
                                      system->stock_file_name, data_file);
         }

         /*This has to go before the string to price, as strtok is not
          * reentrant, and string to price uses it.
          * Load the stock quantity*/
         token = strtok(NULL, DATA_DELIMITER);
         if (token == NULL) {

            return file_error_message("Stock Quantity", line_number,
                                      system->stock_file_name, data_file);
         }
         /* Convert the stock quantity and validate */
         no_error = to_int(token, &onhand);

         /* Check that the integer conversion went successfully */
         if (onhand < 0) {
            no_error = FALSE;
         } else if (!no_error) {

            return file_error_message("Stock Quantity", line_number,
                                      system->stock_file_name, data_file);
         } else {
            stock_item.on_hand = onhand;
         }

         /* Convert the price we loaded previously and validate*/
         no_error = string_to_price(&(stock_item.price), price);

         if (!no_error) {

            return file_error_message("Price", line_number,
                                      system->stock_file_name, data_file);
         }


         /*
         print_stock(stock_item);
         */

         /* Add the stock and validate that it succeeded */
         stock_added = add_stock(stock_item, system);
         if (!stock_added) {
            printf("Error encountered and stock could not be added. Please "
                           "try again, or check your file syntax.\n");
            fclose(data_file);
            return FALSE;
         }

      } else {
         /* If incorrect number of delims tell them */
         printf("Line %d has wrong number of fields:  %s\n",
                line_number, current_line);
         fclose(data_file);
         return FALSE;
      }
   }
   /* Checking for a final line that does not end with a new line. Will not
    * be tokenised, so we current line will not have \0 added during
    * tokenisation after the ID, and will be a longer line. We could load the
    * last line here, but as stated in specification and error message, may be
    * indicative of other problems
    */
   if (strlen(current_line) > IDLEN) {
      fclose(data_file);
      printf("The last line of %s does not end in a new line, and may be "
                     "indicative of file corruption. Loading will halt "
                     "until file is fixed\n", system->stock_file_name);
      return FALSE;
   }
   /* If the reading failed before reaching the end of file, something went
    * wrong */
   if (!feof(data_file)) {
      fclose(data_file);
      return FALSE;
   }
   fclose(data_file);

   return TRUE;
}

/**
 * Loads the coins from filename into system and validates
 * @param system Load into the cash regsiter in system
 * @param filename The filename to load coins from
 * @return Whether the laod operation succeded
 */
BOOLEAN load_coins(struct ppd_system *system, const char *filename) {
   FILE *coin_file;
   char current_line[COIN_LINE_LEN + EXTRACHARS], *token;
   int denom_value, amount, line_number = 0;

   /* set all the balanced to 0 before loading (should have been initialised
    * prior anyway)*/
   void_balances(system->cash_register);

   /* If loading from a file, load from a file */
   if (system->coin_from_file == TRUE) {
      /* Open file and validate */
      coin_file = fopen(system->coin_file_name, "r");
      if (coin_file == NULL) {
         printf("Invalid coin file. No coins were loaded.\n");
         return FALSE;
      }

      /* As long as there are more lines, extract them */
      while (!feof(coin_file) && read_file_input(current_line, COIN_LINE_LEN,
                                                 coin_file)) {
         /* Increment line counter for error messages */
         line_number++;

         /* If the number of delims is correct keep going */
         if (count_delims(COIN_DELIM, current_line) == 1) {
            /* Tokenise the line, read the coin type and validate */
            token = strtok(current_line, COIN_DELIM);
            if (token == NULL) {
               return file_error_message("Coin Type", line_number,
                                         system->coin_file_name, coin_file);
            }

            /* Convert the coin type and validate */
            if (!to_int(token, &denom_value) ||
                !is_valid_value(denom_value)) {
               return file_error_message("Coin Type", line_number,
                                         system->coin_file_name, coin_file);
            }
            /* Load the quantity token */
            token = strtok(NULL, COIN_DELIM);
            if (token == NULL) {
               return file_error_message("Coin Quantity", line_number,
                                         system->coin_file_name, coin_file);
            }

            /* Convert and validate the quantity */
            if (!to_int(token, &amount) || amount < 0) {
               return file_error_message("Coin Quantity", line_number,
                                         system->coin_file_name, coin_file);
            }
            /* If the type of coin we are trying to add is already in the
             * register, we have loaded it before. Error */
            if (count_coins(system->cash_register, value_to_denom
                    (denom_value)) != 0) {
               return file_error_message("Coin: duplicate - already added",
                                         line_number,
                                         system->coin_file_name, coin_file);

            }

            /* Add the coin and check for errors */
            if (!add_coin_val(system->cash_register, denom_value, amount)) {
               printf("Coin adding on line %d failed. Try loading again, "
                              "and check file syntax", line_number);
               return FALSE;
            }
         } else {
            system->coin_from_file = FALSE;
            return file_error_message("Number of fields", line_number,
                                      system->coin_file_name, coin_file);
         }

      }
      /* Checking for a non new line terminated file */
      if (count_delims(COIN_DELIM, current_line) != 0) {
         fclose(coin_file);
         printf("The last line of %s does not end in a new line, and may be "
                        "indicative of file corruption. Loading will halt "
                        "until file is fixed\n", system->coin_file_name);
         return FALSE;
      }
      /* If the reading ended before file end, quit and return false */
      if (!feof(coin_file)) {
         fclose(coin_file);

         return FALSE;
      }
      fclose(coin_file);

   }
/* If there were no errors in loading , or we don't have to load, return true */
   return TRUE;

}

/**
 * Frees all the malloced items in system.
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
/**
 * Reads a single line from file stream file into the output string buffer
 * @param buffer The output string
 * @param length The expected maximum length of the line to read
 * @param file The file to read from
 * @return Whether the line was read with no errors
 */
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

/**
 * Reads an int from stdin and validates it.
 * @param output the int to be read
 * @return Whether the int read was valud or not
 */
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

/**
 * Convers an string to an int
 * @param input The string to be converted to an int
 * @param output the output int
 * @return Whether the int was succesfully converted with no errors
 */
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

/**
 * Converts a delimited dollars.cents string into a price struct. Uses strtok
 * @param price_amount the output struct
 * @param price_input the input string
 * @return Whether the input string was valid and convesion succesful. True
 * if succeeds, false if not
 */
BOOLEAN string_to_price(struct price *price_amount, char *price_input) {
   char *token = NULL;
   int amount;
   BOOLEAN int_success;
   /* strtok_r would make this easier/safer if the calling function also uses
    * strtok, however not C99 */
   /* Tokenise the string and extract the dollar amount*/
   token = strtok(price_input, PRICEDELIM);
   /* Convert, validate and set the dollar amount */
   int_success = to_int(token, &amount);
   if (token != NULL && int_success && amount < MAX_DOLLARS_PRICE) {
      price_amount->dollars = amount;
   } else {
      return FALSE;
   }

   /* Do the same for the cents amount */
   token = strtok(NULL, PRICEDELIM);
   int_success = to_int(token, &amount);

   /* Check if the cents amount is less than the maximum dollar amount, a
    * valid increment of cents and integer conversion completed */
   if (token != NULL && int_success && amount < 100 && (amount % MINUMUM_DENOM)
                                                       == 0) {
      price_amount->cents = amount;
   } else {
      return FALSE;
   }

   /* Tokenise again. If the string had only one token is null */
   token = strtok(NULL, PRICEDELIM);

   /* If the correct number of tokens and everything before succeded/ didn't
    * return false, return true */
   if (token == NULL) {
      return TRUE;
   } else {
      return FALSE;
   }
}

/**
 * Converts a price to an integer in cents
 * @param price The input price
 * @param cents The total number of cents the price represents
 * @return whether the conversion was succesfull
 */
BOOLEAN price_to_int(struct price *price, int *cents) {
   if (price == NULL) {
      return FALSE;
   }
   *cents = price->cents + (price->dollars * CENTS_IN_DOLLAR);
   return TRUE;
}

/**
 * Renames a file to have a BACKUP_SUFFIC extension, or reverses the process
 * @param name The name of the file to be renamed
 * @param reverse if false renames to *BACKUP_SUFFIX, if true renames from
 * *BACKUP_SUFFIX to original name
 * @return Whether the rename was succesfull
 */
BOOLEAN rename_file(const char *name, BOOLEAN reverse) {
   char *file_bak = NULL;
   BOOLEAN worked;
   /* Malloc the new name */
   size_t size;
   size = (sizeof(char) * (strlen(name) + strlen(BACKUP_SUFFIX) + EXTRACHARS));
   file_bak = malloc(size);
   /* Create the name with .bak appended */
   sprintf(file_bak, "%s%s", name, BACKUP_SUFFIX);

   /* Rename */
   if (reverse) {
      worked = rename(file_bak, name);
   } else {
      worked = rename(name, file_bak);
   }
   /* Free the file name */
   free(file_bak);

   return worked;
}

/* Only functions on single character delimiters */
/**
 * A recursive function to count the number of occurences of a character in a
 * string
 * @param delim The delimiter to be analysed. Only the first character is used.
 * @param string The string to search for delimiters in
 * @return The number of delimiters in the string
 */
int count_delims(char *delim, char *string) {
   /* If the delim string is the wrong length error */
   if (strlen(delim) != 1)return -1;
   /* If the string is ended return 0 */
   if (*string == NULL_TERMINATOR) {
      return 0;
   }
   /* If we find a delim add 1 to the count and increment the string */
   if (*string == delim[0]) {
      return 1 + count_delims(delim, ++string);
   }
   /* IF we don't find anything increment the string */
   return count_delims(delim, ++string);
}

/**
 * Prints an error message, closes the file stream and returns false
 * @param field_type The type of invalid field. Printed in error message
 * @param line_number The line numebr of the file that the error occured on
 * @param file_name The name of the file the error occured in
 * @param file The file to close
 * @return Always FALSE
 */
BOOLEAN file_error_message(char *field_type, int line_number, const char
*file_name, FILE *file) {
   fprintf(stderr, "Invalid %s%s%s when loading line %s%d%s in file %s. Item "
                   "was not added\n",
           F_CYAN, field_type, COLOUR_RESET, F_CYAN, line_number, COLOUR_RESET,
           file_name);
   fclose(file);
   return FALSE;
}