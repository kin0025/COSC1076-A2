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

#include <stdio.h>
#include "ppd_main.h"

#ifndef PPD_UTILITY
#define PPD_UTILITY
#define DATA_DELIMITER "|"
/**
 * @file ppd_utility.h defines some helper functions for the rest of your
 * program. In particular, it declares three of the most important 
 * functions for your application: @ref load_data which loads the data
 * in from files, @ref system_init which initialises the system to a 
 * known safe state and @ref system_free which frees all memory when you
 * are going to quit the application.
 **/

/**
 * the function to call for buffer clearing. This was discussed extensively
 * for assignment 1
 **/
void read_rest_of_line(void);

/**
 * Initialise the system to a known safe state. Before you post on the
 * discussion board about this, have a look at the structures as defined
 * in ppd_stock.h, ppd_coin.h and ppd_main.h. Given that nothing is 
 * initialized by the system if it
 * is a local variable, what would be good starting values for each of 
 * these. You already have some experience with this from assignment 
 * 1 to help you.
 **/
BOOLEAN system_init(struct ppd_system *);

/**
 * loads the stock file's data into the system. This needs to be 
 * implemented as part of requirement 2 of the assignment specification.
 **/
BOOLEAN load_stock(struct ppd_system *, const char *);

/**
 * loads the contents of the coins file into the system. This needs to
 * be implemented as part 1 of requirement 18.
 **/
BOOLEAN load_coins(struct ppd_system *, const char *);

/**
 * The base used in strtol
 */
#define BASE 10

/**
 * The number of delims in the stock file
 */
#define NUMBER_STOCK_DELIMS 4

/**
 * The number of delims in the coin file
 */
#define NUMBER_COIN_DELIMS 1

/**
 * The maximum number of dollars (not including) an item can cost
 */
#define MAX_DOLLARS_PRICE 100

/**
 * The suffix to be added on to backups
 */
#define BACKUP_SUFFIX ".bak"

/**
 * free all memory that has been allocated. If you are struggling to
 * find all your memory leaks, compile your program with the -g flag
 * and run it through valgrind. An important thing to think about here:
 * as malloc() returns a memory address to the first byte allocated, you
 * must pass each of these memory addresses to free, and no other 
 * memory addresses.
 **/
void system_free(struct ppd_system *);

/**
 * Reads an int from stdin and validates it.
 * @param output the int to be read
 * @return Whether the int read was valud or not
 */
BOOLEAN read_int(int *output);

/**
 * Convers an string to an int
 * @param input The string to be converted to an int
 * @param output the output int
 * @return Whether the int was succesfully converted with no errors
 */
BOOLEAN to_int(char *input, int *output);

/** Returns false if user presses enter or EOF. Enters input into the buffer
 * file, with expected length of length*/
BOOLEAN read_user_input(char *buffer, int length);

/**
 * Reads a single line from file stream file into the output string buffer
 * @param buffer The output string
 * @param length The expected maximum length of the line to read
 * @param file The file to read from
 * @return Whether the line was read with no errors
 */
BOOLEAN read_file_input(char *buffer, int length, FILE *file);

/**
 * Converts a delimited dollars.cents string into a price struct. Uses strtok
 * @param price_amount the output struct
 * @param price_input the input string
 * @return Whether the input string was valid and convesion succesful. True
 * if succeeds, false if not
 */
BOOLEAN string_to_price(struct price *price_amount, char *price_input);

/**
 * Converts a price to an integer in cents
 * @param price The input price
 * @param cents The total number of cents the price represents
 * @return whether the conversion was succesfull
 */
BOOLEAN price_to_int(struct price *price, int *cents);

/**
 * Renames a file to have a BACKUP_SUFFIX extension, or reverses the process
 * @param name The name of the file to be renamed
 * @param reverse if false renames to .bak, if true renames from .bak to
 * original name
 * @return Whether the rename was succesfull
 */
BOOLEAN rename_file(const char *name, BOOLEAN reverse);

/**
 * A recursive function to count the number of occurences of a character in a
 * string
 * @param delim The delimiter to be analysed. Only the first character is used.
 * @param string The string to search for delimiters in
 * @return The number of delimiters in the string
 */
int count_delims(char *delim, char *string);

/**
 * Prints an error message, closes the file stream and returns false
 * @param field_type The type of invalid field. Printed in error message
 * @param line_number The line numebr of the file that the error occured on
 * @param file_name The name of the file the error occured in
 * @param file The file to close
 * @return Always FALSE
 */
BOOLEAN file_error_message(char *field_type, int line_number,
                           const char *file_name, FILE *file);

#endif
